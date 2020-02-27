#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <random>
#include <ctime>

#include "UI.h"

class LoadingScreen : public UI {
    sf::Font m_roboto_regular, m_roboto_bold;
    sf::Text m_title_text, m_description_text;

    int m_gradient_step = -2;

public:
    LoadingScreen(sf::RenderWindow & window);

    virtual void events();
    virtual void calc();
    virtual void draw();
};

#endif
