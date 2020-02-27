#ifndef CARD_H
#define CARD_H

#include <SFML/Graphics.hpp>

#include "CLog.h"

class Card {
    friend class Round;
    friend class Sequence;
    friend class CUI;
    friend class GamePlay;
    friend class Human;
    
    enum Power {
        _2, _3, _4, _5, _6, _7, _8, _9, _10, _J, _Q, _K, _A
    } m_power;
    enum Color {
        PIK, KIER, KARO, TREFL
    } m_color;

    sf::Texture m_texture;
    inline static sf::Texture m_texture_back;
    sf::Sprite m_sprite;

public:
    Card(int power, int color);
};

#endif
