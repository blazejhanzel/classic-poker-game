#include "CGame.h"

using namespace std;

CGame::CGame() {
    srand(time(NULL));
}

CGame::~CGame() {
    int i = 0;
    for (i = 0; i < m_players.size(); i++)
        delete m_players[i];
    for (i = 0; i < m_tokens.size(); i++)
        delete m_tokens[i];
    for (i = 0; i < m_deck.size(); i++)
        delete m_deck[i];
    delete m_table;
    CLog::instance() << "All objects deleted and memory freed";
}

void CGame::start(Human * human, int players) {
    // Step 1: "Register" all players in game
    for (int i = 0; i < players; i++) {
        if (i == 0) { // human id
            m_players.push_back((Player*) human);
            CLog::instance() << "Registered player #" + to_string(i) + " type: Human";
        }
        else {
            m_players.push_back(new AI);
            CLog::instance() << "Registered player #" + to_string(i) + " type: AI";
        }
    }

    // Step 2: "Generate" start tokens
    m_tokens.push_back(new Token(Token::Type::DEALER));
    CLog::instance() << "Generated Dealer token putted into tokens vector";
    int summary_cash = 0, a = 0; // amount
    for (a = 0; a < 2*players; a++) {
        m_tokens.push_back(new Token(Token::Type::BLACK));
        summary_cash += Token::Type::BLACK;
    }
    for (a = 0; a < 2*players; a++) {
        m_tokens.push_back(new Token(Token::Type::BLUE));
        summary_cash += Token::Type::BLUE;
    }
    for (a = 0; a < 5*players; a++) {
        m_tokens.push_back(new Token(Token::Type::WHITE));
        summary_cash += Token::Type::WHITE;
    }
    for (a = 0; a < 6*players; a++) {
        m_tokens.push_back(new Token(Token::Type::RED));
        summary_cash += Token::Type::RED;
    }
    for (a = 0; a < 8*players; a++) {
        m_tokens.push_back(new Token(Token::Type::GREEN));
        summary_cash += Token::Type::GREEN;
    }
    CLog::instance() << "Generated " + to_string(m_tokens.size()) + " tokens with summary value of: $" + to_string(summary_cash);

    // Step 3: "Give" all tokens to players
    vector < vector < Token * > > starter_pack;
    starter_pack.resize(players);
    int player_id_to_give_cash = 0;
    for (int i = 0; i < m_tokens.size(); i++) {
        switch (m_tokens[i]->m_type) {
            case Token::Type::DEALER: {
                srand(time(NULL));
                int player_id_to_give = rand()%players;
                starter_pack[player_id_to_give].push_back(m_tokens[i]);
                CLog::instance() << "Dealer token was given to player " + to_string(player_id_to_give);
                break;
            }
            case Token::Type::GREEN:
            case Token::Type::RED:
            case Token::Type::WHITE:
            case Token::Type::BLUE:
            case Token::Type::BLACK: {
                starter_pack[player_id_to_give_cash].push_back(m_tokens[i]);
                player_id_to_give_cash = (player_id_to_give_cash+1)%players;
                break;
            }
            default: {
                CLog::instance() << "Undefined type of Token want to be given for player, skipping";
                break;
            }
        }
    }
    for (int i = 0; i < players; i++) {
        m_players[i]->getTokens(starter_pack[i]);
        CLog::instance() << "Player #" + to_string(i) + " get tokens with summary value of: $" + to_string(calculateTokensValue(starter_pack[i]));
    }

    // Step 4: "Generate" deck (52 cards)
    int cards = 52;
    for (int i = 0; i < cards; i++) {
        m_deck.push_back(new Card(i%13, (int)(i/13)));
        //CLog::instance() << "Put card #" + to_string(i) + " " + to_string(i%13+2) + "/" + to_string((int)(i/13)) + " in deck";
    }
    random_shuffle(m_deck.begin(), m_deck.end());
    CLog::instance() << "Putted " + to_string(cards) + " cards in to deck";

    // Step 5: "Prepare" table for game
    m_table = new Table;
    CLog::instance() << "New table for game created";

    // Step 6: Make the game begin, run rounds until end
    while (!m_end_signal) {
        m_round = new Round(m_table, m_players, m_deck);
        m_round->begin();
        // m_gamestate = GAMEOVER;
        // while (m_gamestate == GAMEOVER) {
        //     Sleep(500); // don't harm your CPU
        // }
        // if (m_gamestate == GAMEPLAY) {
            delete m_round;
            m_round = nullptr;
        //     random_shuffle(m_deck.begin(), m_deck.end());
        // } else {
            end();
        // }
    }
}

void CGame::end() {
    m_end_signal = true;
    CLog::instance() << "End signal hitted";
}
