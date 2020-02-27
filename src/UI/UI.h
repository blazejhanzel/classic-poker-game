#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

#include "../CLog.h"

class UI {
protected:
    sf::RenderWindow * m_window;
    
public:
    UI(sf::RenderWindow & window) : m_window(&window) {}

    virtual void events() = 0;
    virtual void calc() = 0;
    virtual void draw() = 0;
};

#endif
