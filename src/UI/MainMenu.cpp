#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow & window) : UI(window) {
    // LOAD FONTS
    if (!m_roboto_regular.loadFromFile("res/fonts/Roboto-Regular.ttf"))
        CLog::instance() << "Error loading font";
    if (!m_roboto_bold.loadFromFile("res/fonts/Roboto-Bold.ttf"))
        CLog::instance() << "Error loading font";
    if (!m_roboto_italic.loadFromFile("res/fonts/Roboto-Italic.ttf"))
        CLog::instance() << "Error loading font";


    // INITIALIZE TEXTS
    m_title_text.setFont(m_roboto_bold);
    m_author_text.setFont(m_roboto_italic);
    m_newgame_text.setFont(m_roboto_regular);
    m_quit_text.setFont(m_roboto_regular);

    m_title_text.setString("Poker dobierany");
    m_author_text.setString(L"by Błażej Hanzel");
    m_newgame_text.setString("Nowa gra");
    m_quit_text.setString(L"Wyjdź");

    m_title_text.setCharacterSize(64);
    m_author_text.setCharacterSize(24);
    m_newgame_text.setCharacterSize(48);
    m_quit_text.setCharacterSize(48);

    m_title_text.setFillColor(sf::Color::White);
    m_author_text.setFillColor(sf::Color::White);
    m_newgame_text.setFillColor(sf::Color::White);
    m_quit_text.setFillColor(sf::Color::White);


    calc();
}

void MainMenu::events() {
    if (m_newgame_bound.contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
        m_newgame_text.setFillColor(sf::Color::Yellow);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            CLog::instance() << "New game starting right now";
            CGame::instance().m_gamestate = CGame::GameState::GAMEPLAY;
        }
    } else {
        m_newgame_text.setFillColor(sf::Color::White);
    }
    
    if (m_quit_bound.contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
        m_quit_text.setFillColor(sf::Color::Yellow);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            CLog::instance() << "End signal hitted";
            m_window->close();
        }
    } else {
        m_quit_text.setFillColor(sf::Color::White);
    }
}

void MainMenu::calc() {
    // SET POSITIONS
    m_title_text.setPosition(m_window->getSize().x / 2 - m_title_text.getLocalBounds().width / 2, 50);
    m_author_text.setPosition(m_window->getSize().x / 2 - m_author_text.getLocalBounds().width / 2, 62 + m_title_text.getLocalBounds().height);
    m_newgame_text.setPosition(m_window->getSize().x / 2 - m_newgame_text.getLocalBounds().width / 2, 384);
    m_quit_text.setPosition(m_window->getSize().x / 2 - m_quit_text.getLocalBounds().width / 2, 384 + 24 + m_newgame_text.getLocalBounds().height);

    // SET BOUNDING BOXES
    m_newgame_bound = m_newgame_text.getGlobalBounds();
    m_quit_bound = m_quit_text.getGlobalBounds();
}

void MainMenu::draw() {
    m_window->clear();
    m_window->draw(m_title_text);
    m_window->draw(m_author_text);
    m_window->draw(m_newgame_text);
    m_window->draw(m_quit_text);
}
