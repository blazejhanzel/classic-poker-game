#ifndef TOKEN_H
#define TOKEN_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "CLog.h"

class Token {
    friend class CGame;
    friend class Player;
    friend class Round;
    friend class CUI;
    friend class AI;
    friend class GamePlay;
    friend class Human;

    enum Type {
        DEALER, // rozdający // player which starts turn
        // money tokens with theirs values in $
        GREEN = 50,
        RED = 100,
        WHITE = 200,
        BLUE = 500,
        BLACK = 1000
    } m_type;

    friend int calculateTokensValue (std::vector < Token * > tokens) {
        int summary_cash = 0;
        for (int i = 0; i < tokens.size(); i++) {
            if ((int)tokens[i]->m_type != DEALER)
                summary_cash += (int)tokens[i]->m_type;
        }
        return summary_cash;
    }

    sf::Texture m_texture;
    sf::Sprite m_sprite;

public:
    Token(Type type); // initialize token with choosen type
};

#endif
