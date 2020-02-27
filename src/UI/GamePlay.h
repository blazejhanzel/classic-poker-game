#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "UI.h"
#include "../Table.h"
#include "../Token.h"
#include "../CGame.h"

class GamePlay : public UI {
    sf::Font m_roboto_bold, m_roboto_regular;
    sf::Text m_token_text[4][5], m_tokenontablevalue_text, m_playermove_text[4];

    sf::Texture m_leg_token_texture[5];
    sf::Sprite m_leg_token_sprite[5];
    sf::Text m_leg_title_text, m_leg_token_text[5];

    int m_players_count = 0;

public:
    GamePlay(sf::RenderWindow & window);

    virtual void events();
    virtual void calc();
    virtual void draw();
};

#endif
