#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "UI.h"
#include "../CGame.h"

class GameOver : public UI {
    sf::Font m_roboto_bold, m_roboto_regular, m_roboto_italic;
    sf::Text m_title_text, m_winlost_text, m_score_text, m_newround_text, m_quit_text;
    sf::FloatRect m_newround_bound, m_quit_bound;

public:
    GameOver(sf::RenderWindow & window);

    virtual void events();
    virtual void calc();
    virtual void draw();
};

#endif
