#include "CUI.h"

using namespace std;

CUI::CUI() {
    m_window.create(sf::VideoMode(1024, 768), "Poker dobierany", sf::Style::Titlebar | sf::Style::Close);
    // m_windev.create(sf::VideoMode(969, 740), "Konsola developerska", sf::Style::Titlebar | sf::Style::Close);
    // m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2 - 1024 - 30 + 150, sf::VideoMode::getDesktopMode().height/2 - 768/2)); // use only in development stage
    // m_windev.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2 + 30 + 150, sf::VideoMode::getDesktopMode().height/2 - 740/2));
    m_window.setFramerateLimit(60);
    // m_windev.setFramerateLimit(60);

    // LOAD FONTS
    if (!m_roboto_regular.loadFromFile("res/fonts/Roboto-Regular.ttf"))
        CLog::instance() << "Error loading font";

    // m_gameover = new GameOver(m_window);
}

CUI::~CUI() {
    if (m_mainmenu) delete m_mainmenu;
    if (m_gameplay) delete m_gameplay;
    // if (m_gameover) delete m_gameover;
}

void CUI::startLogic() {
    CGame::instance().start(m_human);
}

void CUI::windowRender() {
    if (CGame::instance().m_gamestate == CGame::GameState::MAIN_MENU) {
        if (m_mainmenu == nullptr)
            m_mainmenu = new MainMenu(m_window);

        m_mainmenu->events();
        m_mainmenu->draw();
    } else if (CGame::instance().m_gamestate == CGame::GameState::GAMEPLAY) {
        if (CGame::instance().m_table == nullptr && m_gameplay == nullptr) { // m_table is loading
            m_gameplay = new LoadingScreen(m_window);
        }
        else if (CGame::instance().m_table != nullptr && m_gameplay_loaded == false) { // m_table is loaded
            m_gameplay_loaded = true;
            if (m_gameplay != nullptr)
                delete m_gameplay;
            m_gameplay = new GamePlay(m_window);
        }

        if (m_gameplay) {
            m_gameplay->calc(); // this need to be calculated every frame, because cards move around table
            if (m_gameplay_loaded)
                m_human->calc();
            m_gameplay->events();
            if (m_gameplay_loaded)
                m_human->events();
            m_gameplay->draw();
            if (m_gameplay_loaded)
                m_human->draw();
        }
    }
    // else if (CGame::instance().m_gamestate == CGame::GameState::GAMEOVER) {
    //     if (m_mainmenu) delete m_mainmenu;
    //     if (m_gameplay) delete m_gameplay;
    //     m_gameover->events();
    //     m_gameover->calc();
    //     m_gameover->draw();
    // }

    m_window.display();
}

void CUI::windevRender() {
    if (CGame::instance().m_table != nullptr) {
        int player_count = CGame::instance().m_players.size();
        sf::Text player_name[player_count], stack_text;

        // m_windev.setSize(sf::Vector2u(969, (player_count + 1) * 148)); // use when different than 4 players

        for (int i = 0; i < player_count; i++) {
            player_name[i].setFont(m_roboto_regular);
            player_name[i].setString("Player #" + to_string(i));
            player_name[i].setCharacterSize(24);
            player_name[i].setFillColor(sf::Color::Black);
            player_name[i].setPosition(0, i * 148);

            for (int j = 0; j < CGame::instance().m_players[i]->m_cards.size(); j++) {
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setTexture(CGame::instance().m_players[i]->m_cards[j]->m_texture);
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setRotation(0.0f);
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setPosition(sf::Vector2f(j * 78 + 4, i * 148 + 30));
            }
            for (int j = 0; j < CGame::instance().m_players[i]->m_tokens.size(); j++) {
                CGame::instance().m_players[i]->m_tokens[j]->m_sprite.setPosition(sf::Vector2f(j % 13 * (40 + 4) + 4 + 5 * 78, i * 148 + 30 + (45 * (int) (j / 13))));
            }
        }

        stack_text.setFont(m_roboto_regular);
        stack_text.setString("Card stack");
        stack_text.setCharacterSize(24);
        stack_text.setFillColor(sf::Color::Black);
        stack_text.setPosition(0, player_count * 148);
        if (CGame::instance().m_table) {
            for (int i = 0; i < CGame::instance().m_table->m_cardstack.size(); i++) {
                CGame::instance().m_table->m_cardstack[i]->m_sprite.setTexture(CGame::instance().m_table->m_cardstack[i]->m_texture);
                CGame::instance().m_table->m_cardstack[i]->m_sprite.setRotation(0.0f);
                CGame::instance().m_table->m_cardstack[i]->m_sprite.setPosition(sf::Vector2f((i * 69 * 0.25) + 4, player_count * 148 + 30));
            }
        }

        m_windev.clear(sf::Color::White);
        for (int i = 0; i < player_count; i++) {
            m_windev.draw(player_name[i]);
            for (int j = 0; j < CGame::instance().m_players[i]->m_cards.size(); j++) {
                m_windev.draw(CGame::instance().m_players[i]->m_cards[j]->m_sprite);
            }
            for (int j = 0; j < CGame::instance().m_players[i]->m_tokens.size(); j++) {
                m_windev.draw(CGame::instance().m_players[i]->m_tokens[j]->m_sprite);
            }
        }
        m_windev.draw(stack_text);
        if (CGame::instance().m_table)
            for (int i = 0; i < CGame::instance().m_table->m_cardstack.size(); i++)
                m_windev.draw(CGame::instance().m_table->m_cardstack[i]->m_sprite);
        m_windev.display();
    }
}

void CUI::start() {
    m_human = new Human(m_window);

    sf::Thread logic_thr(&startLogic);

    CGame::GameState last_gamestate = CGame::GameState::MAIN_MENU;
    while (m_window.isOpen()) {
        // check for running game thread
        if (CGame::instance().m_gamestate == CGame::GameState::GAMEPLAY && last_gamestate == CGame::GameState::MAIN_MENU) {
            last_gamestate = CGame::GameState::GAMEPLAY;
            logic_thr.launch();
        }

        // event handling
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                logic_thr.terminate();
                m_window.close();
            }
        }
        // while (m_windev.pollEvent(event)) {
        //     if (event.type == sf::Event::Closed)
        //         m_windev.close();
        // }

        // window drawing
        windowRender();
        // windevRender();
    }
}
