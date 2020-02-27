#include "Player.h"

using namespace std;

void Player::getCard(Card * card) {
    m_cards.push_back(card);
}

void Player::getTokens(vector < Token * > tokens) {
    srand(time(NULL));
    if (m_tokens.size() == 0)
        m_tokens = tokens;
    else
        m_tokens.insert(m_tokens.end(), tokens.begin(), tokens.end());
    random_device rd;
    mt19937 g(rd());
    shuffle(m_tokens.begin(), m_tokens.end(), g);

    #ifdef _WIN32
    // this one sleeps thread for more than one second to get srand() work better / not necessary but good to have
    Sleep(1200);
    #endif
}

bool Player::isDealer() {
    for (int i = 0; i < m_tokens.size(); i++) {
        if (m_tokens[i]->m_type == Token::Type::DEALER)
            return true;
    }
    return false;
}

vector < Token * > Player::ante() {
    vector < Token * > ante_return;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < m_tokens.size(); j++) {
            switch (i) {
                case 0: { // Token::Type::WHITE
                    if (m_tokens[j]->m_type == Token::Type::WHITE) {
                        ante_return.push_back(m_tokens[j]);
                        m_tokens.erase(m_tokens.begin() + j);
                        return ante_return;
                    }
                    break;
                }
                case 1: { // Token::Type::RED
                    if (m_tokens[j]->m_type == Token::Type::RED) {
                        ante_return.push_back(m_tokens[j]);
                        m_tokens.erase(m_tokens.begin() + j);
                        if (calculateTokensValue(ante_return) >= 200)
                            return ante_return;
                    }
                    break;
                }
                case 2: { // Token::Type::GREEN
                    if (m_tokens[j]->m_type == Token::Type::GREEN) {
                        ante_return.push_back(m_tokens[j]);
                        m_tokens.erase(m_tokens.begin() + j);
                        if (calculateTokensValue(ante_return) >= 200)
                            return ante_return;
                    }
                    break;
                }
                case 3: { // Token::Type::BLUE
                    if (m_tokens[j]->m_type == Token::Type::BLUE) {
                        ante_return.push_back(m_tokens[j]);
                        m_tokens.erase(m_tokens.begin() + j);
                        return ante_return;
                    }
                    break;
                }
                case 4: { // Token::Type::BLACK
                    if (m_tokens[j]->m_type == Token::Type::BLACK) {
                        ante_return.push_back(m_tokens[j]);
                        m_tokens.erase(m_tokens.begin() + j);
                        return ante_return;
                    }
                    break;
                }
                default: {
                    CLog::instance() << "Error in class Player: ante wants undefined Type to be returned";
                }
            }
        }
    }

    return ante_return;
}
