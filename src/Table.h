#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <SFML/Graphics.hpp>

#include "Card.h"
#include "Token.h"
#include "CLog.h"

class Table {
    friend class Round;
    friend class CUI;
    friend class GamePlay;
    
    std::vector < Card * > m_cardstack; // stack of cards free to use
    std::vector < Token * > m_tokens; // cash token on table

    sf::RectangleShape m_card_placeholder[20]; // place where cards will be drawen
    sf::RectangleShape m_cardstack_placeholder;

public:
    Table();
};

#endif
