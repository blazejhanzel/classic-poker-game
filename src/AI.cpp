#include "AI.h"

using namespace std;

enum Player::PossibleMoves AI::auction(int value_to_beat, vector < Token * > & auction_return) {
    Sleep(1000);

    for (int i = m_tokens.size() - 1; i >= 0; i--) {
        if (m_tokens[i]->m_type != Token::Type::DEALER) {
            auction_return.push_back(m_tokens[i]);
            m_tokens.erase(m_tokens.begin() + i);
            if (calculateTokensValue(auction_return) > value_to_beat) {
                if (value_to_beat == 0) { // first move in auction
                    srand(time(NULL));
                    int rnd = rand()%2;
                    if (rnd == 0)
                        return PossibleMoves::RAISE;
                    else
                        return PossibleMoves::CALL;
                }
                else
                    return PossibleMoves::RAISE;
            }
            if (calculateTokensValue(auction_return) == value_to_beat)
                return PossibleMoves::CALL;
        }
    }

    // if doesn't have enough token to beat value: "all in" will do
    return PossibleMoves::ALLIN;
}

int AI::cardChange(vector < Card * > & cardstack) {
    srand(time(NULL));
    int how_much_to_change = rand()%6;

    for (int i = 0; i < how_much_to_change; i++) {
        cardstack.push_back(m_cards[i]);
        m_cards.erase(m_cards.begin()+i);
        getCard(cardstack[0]);
        cardstack.erase(cardstack.begin());
    }

    #ifdef _WIN32
    Sleep(1200); // sleep for more than one second to get better random values
    #endif

    return how_much_to_change;
}
