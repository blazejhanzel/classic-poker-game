#include "LoadingScreen.h"

using namespace std;

LoadingScreen::LoadingScreen(sf::RenderWindow & window) : UI(window) {
    srand(time(NULL));

    // LOAD FONTS
    if (!m_roboto_regular.loadFromFile("res/fonts/Roboto-Regular.ttf"))
        CLog::instance() << "Error loading font";
    if (!m_roboto_bold.loadFromFile("res/fonts/Roboto-Bold.ttf"))
        CLog::instance() << "Error loading font";


    // INITIALIZE TEXT
    m_title_text.setFont(m_roboto_bold);
    m_description_text.setFont(m_roboto_regular);

    m_title_text.setString(L"Proszę czekać");
    int rnd = rand()%4;
    switch (rnd) {
        default:
        case 0:
            m_description_text.setString(L"Gra jest teraz ładowana...");
            break;
        case 1:
            m_description_text.setString(L"Rozkładamy zielony obrus na stole...");
            break;
        case 2:
            m_description_text.setString(L"Krupier szuka kart...");
            break;
        case 3:
            m_description_text.setString(L"Wkładamy asy do rękawów...");
            break;
    }

    m_title_text.setCharacterSize(64);
    m_description_text.setCharacterSize(36);

    m_title_text.setFillColor(sf::Color::White);
    m_description_text.setFillColor(sf::Color::White);


    calc();
}

void LoadingScreen::events() {

}

void LoadingScreen::calc() {
    // SET POSITIONS
    m_title_text.setPosition(m_window->getSize().x / 2 - m_title_text.getLocalBounds().width / 2, m_window->getSize().y / 2 - 12 - m_title_text.getLocalBounds().height);
    m_description_text.setPosition(m_window->getSize().x / 2 - m_description_text.getLocalBounds().width / 2, m_window->getSize().y / 2 + 12);

    // CHANGE TEXT COLOR
    sf::Color color = m_description_text.getFillColor();
    if ((color.r < 128 && color.g < 128 && color.b < 128 && m_gradient_step < 0) || (color.r > 250 && color.g > 250 && color.b > 250 && m_gradient_step > 0)) m_gradient_step *= -1;
    if (m_gradient_step < 0) { // step down
        if (color.r > 128) color.r += m_gradient_step;
        else if (color.g > 128) color.g += m_gradient_step;
        else if (color.b > 128) color.b += m_gradient_step;
    } else { // step up
        if (color.r < 250) color.r += m_gradient_step;
        else if (color.g < 250) color.g += m_gradient_step;
        else if (color.b < 250) color.b += m_gradient_step;
    }
    m_description_text.setFillColor(color);
}

void LoadingScreen::draw() {
    m_window->clear(sf::Color(0, 153/2, 0));
    m_window->draw(m_title_text);
    m_window->draw(m_description_text);
}
