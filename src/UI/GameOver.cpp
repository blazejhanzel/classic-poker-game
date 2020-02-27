#include "GameOver.h"

GameOver::GameOver(sf::RenderWindow & window) : UI(window) {
    // LOAD FONTS
    if (!m_roboto_regular.loadFromFile("res/fonts/Roboto-Regular.ttf"))
        CLog::instance() << "Error loading font";
    if (!m_roboto_bold.loadFromFile("res/fonts/Roboto-Bold.ttf"))
        CLog::instance() << "Error loading font";
    if (!m_roboto_italic.loadFromFile("res/fonts/Roboto-Italic.ttf"))
        CLog::instance() << "Error loading font";

    
    // INITIALIZE TEXTS
    m_title_text.setFont(m_roboto_bold);
    m_winlost_text.setFont(m_roboto_italic);
    m_score_text.setFont(m_roboto_regular);
    m_newround_text.setFont(m_roboto_regular);
    m_quit_text.setFont(m_roboto_regular);

    m_title_text.setString("Koniec rundy");
    m_winlost_text.setString("Wygrywasz/Przegrywasz!");
    m_score_text.setString("0$");
    m_newround_text.setString("Kolejne rozdanie");
    m_quit_text.setString("Koniec gry");

    m_title_text.setCharacterSize(64);
    m_winlost_text.setCharacterSize(48);
    m_score_text.setCharacterSize(24);
    m_newround_text.setCharacterSize(48);
    m_quit_text.setCharacterSize(48);

    m_title_text.setFillColor(sf::Color::White);
    m_winlost_text.setFillColor(sf::Color::White);
    m_score_text.setFillColor(sf::Color::White);
    m_newround_text.setFillColor(sf::Color::White);
    m_quit_text.setFillColor(sf::Color::White);


    calc();
}

void GameOver::events() {
    if (m_newround_bound.contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
        m_newround_text.setFillColor(sf::Color::Yellow);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            CLog::instance() << "New round starting right now";
            CGame::instance().m_gamestate = CGame::GameState::GAMEPLAY;
        }
    } else {
        m_newround_text.setFillColor(sf::Color::White);
    }
    
    if (m_quit_bound.contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
        m_quit_text.setFillColor(sf::Color::Yellow);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            CGame::instance().m_gamestate = CGame::GameState::MAIN_MENU;
        }
    } else {
        m_quit_text.setFillColor(sf::Color::White);
    }
}

void GameOver::calc() {
    // SET POSITIONS
    m_title_text.setPosition(m_window->getSize().x / 2 - m_title_text.getLocalBounds().width / 2, 50);
    m_winlost_text.setPosition(m_window->getSize().x / 2 - m_winlost_text.getLocalBounds().width / 2, 62 + m_title_text.getLocalBounds().height);
    m_score_text.setPosition(m_window->getSize().x / 2 - m_score_text.getLocalBounds().width / 2, 62 + m_title_text.getLocalBounds().height + 12 + m_winlost_text.getLocalBounds().height);
    m_newround_text.setPosition(m_window->getSize().x / 2 - m_newround_text.getLocalBounds().width / 2, 384);
    m_quit_text.setPosition(m_window->getSize().x / 2 - m_quit_text.getLocalBounds().width / 2, 384 + 24 + m_newround_text.getLocalBounds().height);

    // SET BOUNDING BOXES
    m_newround_bound = m_newround_text.getGlobalBounds();
    m_quit_bound = m_quit_text.getGlobalBounds();
}

void GameOver::draw() {
    m_window->clear(sf::Color(0, 153/2, 0));
    m_window->draw(m_title_text);
    m_window->draw(m_winlost_text);
    m_window->draw(m_score_text);
    m_window->draw(m_newround_text);
    m_window->draw(m_quit_text);
}
