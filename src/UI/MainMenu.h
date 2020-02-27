#ifndef MAINMENU_H
#define MAINMENU_H

#include "UI.h"
#include "../CGame.h"

class MainMenu : public UI {
    sf::Font m_roboto_regular, m_roboto_bold, m_roboto_italic;
    sf::Text m_title_text, m_author_text, m_newgame_text, m_quit_text;
    sf::FloatRect m_newgame_bound, m_quit_bound;

public:
    MainMenu(sf::RenderWindow & window);

    virtual void events();
    virtual void calc();
    virtual void draw();
};

#endif
