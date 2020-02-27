#include "Round.h"

using namespace std;

Round::Round(Table * table, vector < Player * > players, vector < Card * > deck) {
    m_table = table;
    m_players = players;
    m_deck = deck;
}

Round::~Round() {
    for (int i = 0; i < m_sequences.size(); i++) {
        delete m_sequences[i];
    }
}

void Round::wait(int ms = 1000) {
    #ifdef _WIN32
    Sleep(ms); // sleep for more than one second to get better random values or to have more time to debug watch
    #endif
}

void Round::begin() {
    CLog::instance() << "New round began";

    // Step 1: Get Ante from player and kick from game this who don't have money
    for (int i = 0; i < m_players.size(); i++) {
        vector < Token * > goten_tokens = m_players[i]->ante();
        if (goten_tokens.size() != 0) {            
            CLog::instance() << "Player #" + to_string(i) + " gave $" + to_string(calculateTokensValue(goten_tokens)) + " for ante";
            m_playermove_string[i] = "Ante: " + to_string(calculateTokensValue(goten_tokens)) + "$";
        }
        else { // player need to be kicked
            m_players.erase(m_players.begin() + i);
            CLog::instance() << "Player #" + to_string(i) + " has not enought cash for ante and was kicked";
            m_playermove_string[i] = "Rezygnacja";
        }
        m_table->m_tokens.insert(m_table->m_tokens.end(), goten_tokens.begin(), goten_tokens.end());
    }

    // Step 2: Card shuffling
    random_device rd;
    mt19937 g(rd());
    shuffle(m_deck.begin(), m_deck.end(), g);

    // Search for dealer
    int dealer_id = 0;
    for (int i = 0; i < m_players.size(); i++) {
        if (m_players[i]->isDealer()) {
            dealer_id = i;
            break;
        }
    }

    // Step 3: Give 5 cards to every player starting from dealer
    for (int i = 0; i < m_players.size(); i++) {
        for (int j = 0; j < 5; j++) {
            m_players[(dealer_id+i)%m_players.size()]->getCard(m_deck.back());
            CLog::instance() << "Player #" + to_string((dealer_id+i)%m_players.size()) + " got " + to_string((int)m_deck.back()->m_power) + "/" + to_string((int)m_deck.back()->m_color) + " card";
            m_deck.pop_back();
            wait(200);
        }
    }

    // Step 4: Put left cards on stack on table
    m_table->m_cardstack = m_deck;
    CLog::instance() << to_string(m_deck.size()) + " cards put on stack on the table";
    wait();

    // Step 5: Player's auctions
    // Step 5.1: Init empty vector of possible moves
    vector < Player::PossibleMoves > move;
    move.resize(m_players.size());
    for (int i = 0; i < m_players.size(); i++) {
        move[i] = Player::PossibleMoves::RAISE; // this is safe, probably, if not, new enum entry is needed
    }

    bool human_fold = false;
    // Step 5.2: Auction // Step 7.1 second auction
    for (int i = 0; i < 4; i++)
        m_playermove_string[i] = "";

    for (int step = 0; step < 2; step++) {
        if (step == 1) {
            for (int i = 0; i < m_players.size(); i++) {
                if (move[i] != Player::PossibleMoves::ALLIN)
                    move[i] = Player::PossibleMoves::RAISE; // this is safe, probably, if not, new enum entry is needed
            }
        }

        bool auction_loop = true;
        int value_to_beat = 0;
        while (auction_loop) {

            for (int i = 0; i < m_players.size(); i++) {
                int current_id = (dealer_id+1+i)%m_players.size();

                if (move[current_id] == Player::PossibleMoves::FOLD
                    || move[current_id] == Player::PossibleMoves::CALL
                    || move[current_id] == Player::PossibleMoves::ALLIN)
                    continue; // ommit this player, it's already ended this auction

                vector < Token * > goten_tokens;
                move[current_id] = m_players[current_id]->auction(value_to_beat, goten_tokens);
                
                switch (move[current_id]) {
                    case Player::PossibleMoves::FOLD: {
                        CLog::instance() << "Player #" + to_string(current_id) + " resigned in round";
                        m_playermove_string[current_id] = "Pas";
                        break;
                    }
                    case Player::PossibleMoves::CALL: {
                        CLog::instance() << "Player #" + to_string(current_id) + " calls with $" + to_string(calculateTokensValue(goten_tokens));
                        m_playermove_string[current_id] = "Sprawdzenie: " + to_string(calculateTokensValue(goten_tokens)) + "$";
                        break;
                    }
                    case Player::PossibleMoves::RAISE: {
                        CLog::instance() << "Player #" + to_string(current_id) + " raise with $" + to_string(calculateTokensValue(goten_tokens));
                        m_playermove_string[current_id] = "Podbicie: " + to_string(calculateTokensValue(goten_tokens)) + "$";
                        break;
                    }
                    case Player::PossibleMoves::ALLIN: {
                        CLog::instance() << "Player #" + to_string(current_id) + " is all-in with $" + to_string(calculateTokensValue(goten_tokens));
                        m_playermove_string[current_id] = "All in: " + to_string(calculateTokensValue(goten_tokens)) + "$";
                        break;
                    }
                    default: {
                        CLog::instance() << "Error in auction. Player want to move different than specified in enum.";
                        break;
                    }
                }

                if (calculateTokensValue(goten_tokens) > value_to_beat)
                    value_to_beat = calculateTokensValue(goten_tokens);

                m_table->m_tokens.insert(m_table->m_tokens.end(), goten_tokens.begin(), goten_tokens.end());
            }

            int ready = 0;
            for (int i = 0; i < m_players.size(); i++) { // check if all players are ready
                if (move[i] == Player::PossibleMoves::FOLD
                    || move[i] == Player::PossibleMoves::CALL
                    || move[i] == Player::PossibleMoves::ALLIN) {
                    ready++;
                }
            }
            if (ready >= m_players.size()-1) { // all or one less need to call
                auction_loop = false; // stop loop
            }
        }


        // Step 5.3: Step 7.3: All auctions ended, remove from game player that folded
        // It is safe to do this here, because of probably numerations errors, if deleted earlier
        for (int i = m_players.size()-1; i >= 0; i--) {
            if (move[i] == Player::PossibleMoves::FOLD) {
                m_players.erase(m_players.begin() + i);
                move.erase(move.begin() + i);
                if (i == 0)
                    human_fold = true;
            }
        }

        Sleep(1000);
        // for (int i = 0; i < 4; i++)
            //m_playermove_string[i] = "";

        if (step == 0) {
            // Step 6: Changing carts by players
            for (int i = 0; i < m_players.size(); i++) {
                int how_many = m_players[(dealer_id+1+i)%m_players.size()]->cardChange(m_table->m_cardstack);
                CLog::instance() << "Player #" + to_string((dealer_id+1+i)%m_players.size()) + " changed " + to_string(how_many) + " cards";
                m_playermove_string[(dealer_id+1+i)%m_players.size()] = "Wymiana: " + to_string(how_many);
            }

            Sleep(1000);
            // for (int i = 0; i < 4; i++)
                //m_playermove_string[i] = "";
        }
    }

    // Step 8: For all players who left, create sequence and choose the strongest
    for (int i = 0; i < 4; i++)
        m_playermove_string[i] = "";

    m_sequences.resize(m_players.size());
    for (int i = 0; i < m_players.size(); i++) {
        m_sequences[i] = new Sequence(m_players[i]->m_cards);
        CLog::instance() << "Player #" + to_string(i) + " has " + to_string(m_sequences[i]->m_seq) + " sequence with power " + to_string(m_sequences[i]->m_power);
        for (int j = 0; j < 5; j++) {
            m_players[i]->m_cards[j]->m_sprite.setTexture(m_players[i]->m_cards[j]->m_texture);
        }
        m_playermove_string[(human_fold) ? i+1 : i] = [] (Sequence::SequenceName n)->std::string {
            switch (n) {
                case Sequence::SequenceName::HIGH_CARD:
                    return "Karta";
                case Sequence::SequenceName::ONE_PAIR:
                    return "Para";
                case Sequence::SequenceName::TWO_PAIRS:
                    return "Dwie pary";
                case Sequence::SequenceName::THREE_OF_A_KIND:
                    return "Trzy karty";
                case Sequence::SequenceName::STRAIGHT:
                    return "Strit";
                case Sequence::SequenceName::FLUSH:
                    return "Kolor";
                case Sequence::SequenceName::FULL_HOUSE:
                    return "Full";
                case Sequence::SequenceName::FOUR_OF_A_KIND:
                    return "Kareta";
                case Sequence::SequenceName::STRAIGHT_FLUSH:
                    return "Poker";
                case Sequence::SequenceName::ROYAL_FLUSH:
                    return "Poker kr.";
            }
        } (m_sequences[i]->m_seq);
    }
    // First, we assumpt, that first player is the winner
    int winning_player_id = 0;
    vector < Player * > winning_players;
    winning_players.push_back(m_players[0]);
    for (int i = 1; i < m_players.size(); i++) {
        if (*m_sequences[i] > *m_sequences[winning_player_id]) {
            CLog::instance() << "Player #" + to_string(i) + " > player #" + to_string(winning_player_id);
            winning_player_id = i;
            winning_players.clear();
            winning_players.push_back(m_players[i]);
        }
        else if (*m_sequences[i] == *m_sequences[winning_player_id]) {
            CLog::instance() << "Player #" + to_string(i) + " == player #" + to_string(winning_player_id);
            // winning_player_id = i; // doesn't matter
            winning_players.push_back(m_players[i]);
        }
    }

    // Step 9: Give tokens to winner(s)
    if (winning_players.size() == 1) { // if there is one winner
        winning_players[0]->getTokens(m_table->m_tokens);
    }
    else { // if there are more winners
        vector < Token * > tokens[5];
        vector < Token * > left_tokens;
        for (int i = 0; i < m_table->m_tokens.size(); i++) {
            tokens[(
                [](Token::Type value)->int {
                    switch (value) {
                        case Token::Type::GREEN:
                            return 0;
                        case Token::Type::RED:
                            return 1;
                        case Token::Type::WHITE:
                            return 2;
                        case Token::Type::BLUE:
                            return 3;
                        case Token::Type::BLACK:
                            return 4;
                        default: {
                            CLog::instance() << "Error when dividing tokens, udnefined type reached";
                            return 0;
                        }
                    }
                } (m_table->m_tokens[i]->m_type)
            )].push_back(m_table->m_tokens[i]);
        }
        for (int i = 4; i >= 0; i--) {
            // First, take tokens that will left from dividing
            for (int j = 0; j < (tokens[i].size() % winning_players.size()); j++) {
                left_tokens.push_back(tokens[i][j]);
            }
            // Next, divide all tokens for all winners
            for (int p = 0, t = (tokens[i].size() % winning_players.size()); t < tokens[i].size(); t++, p=(p+1)%winning_players.size()) {
                m_players[p]->getTokens(
                    [] (Token * token)->vector < Token *> {
                        vector < Token * > vec;
                        vec.push_back(token);
                        return vec;
                    } (tokens[i][t])
                );
            }
        }

        // TODO: Need to do something with tokens that lefts
    }
}
