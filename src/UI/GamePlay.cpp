#include "GamePlay.h"

using namespace std;

GamePlay::GamePlay(sf::RenderWindow & window) : UI(window), m_players_count(CGame::instance().m_players.size()) {
    // LOAD FONTS
    if (!m_roboto_bold.loadFromFile("res/fonts/Roboto-Bold.ttf"))
        CLog::instance() << "Error loading font to GamePlay UI";
    if (!m_roboto_regular.loadFromFile("res/fonts/Roboto-Regular.ttf"))
        CLog::instance() << "Error loading font to GamePlay UI";


    // INITIALIZE TEXTS
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            m_token_text[i][j].setFont(m_roboto_bold);
            m_token_text[i][j].setCharacterSize(32);
            m_token_text[i][j].setFillColor(sf::Color::White);
        }
        m_playermove_text[i].setFont(m_roboto_regular);
        m_playermove_text[i].setCharacterSize(32);
        m_playermove_text[i].setFillColor(sf::Color::White);
        m_playermove_text[i].setOutlineColor(sf::Color(192, 0, 0, 48));
        m_playermove_text[i].setOutlineThickness(4);
    }
    m_tokenontablevalue_text.setFont(m_roboto_bold);
    m_tokenontablevalue_text.setCharacterSize(32);
    m_tokenontablevalue_text.setFillColor(sf::Color::White);
    m_tokenontablevalue_text.setOutlineColor(sf::Color::Black);
    m_tokenontablevalue_text.setOutlineThickness(4);


    // INITIALIZE PLACEHOLDERS ON TABLE
    for (int i = 0; i < 20; i++) {
        CGame::instance().m_table->m_card_placeholder[i].setSize(sf::Vector2f(69.f, 99.f));
        CGame::instance().m_table->m_card_placeholder[i].setFillColor(sf::Color(0, 99, 0));
        CGame::instance().m_table->m_card_placeholder[i].setOutlineThickness(2.0f);
        CGame::instance().m_table->m_card_placeholder[i].setOutlineColor(sf::Color(0, 69, 0));
    }
    CGame::instance().m_table->m_cardstack_placeholder.setSize(sf::Vector2f(69.f, 99.f));
    CGame::instance().m_table->m_cardstack_placeholder.setFillColor(sf::Color(0, 99, 0));
    CGame::instance().m_table->m_cardstack_placeholder.setOutlineThickness(2.0f);
    CGame::instance().m_table->m_cardstack_placeholder.setOutlineColor(sf::Color(0, 69, 0));


    // INITIALIZE LEGEND
    m_leg_token_texture[0].loadFromFile("res/textures/token_green.png");
    m_leg_token_texture[1].loadFromFile("res/textures/token_red.png");
    m_leg_token_texture[2].loadFromFile("res/textures/token_white.png");
    m_leg_token_texture[3].loadFromFile("res/textures/token_blue.png");
    m_leg_token_texture[4].loadFromFile("res/textures/token_black.png");

    for (int i = 0; i < 5; i++) {
        m_leg_token_sprite[i].setTexture(m_leg_token_texture[i]);
        m_leg_token_sprite[i].setScale(sf::Vector2(0.5f, 0.5f));
        m_leg_token_text[i].setFont(m_roboto_regular);
        m_leg_token_text[i].setCharacterSize(16);
        m_leg_token_text[i].setFillColor(sf::Color::White);
        m_leg_token_text[i].setString(
            [] (int i)->sf::String {
                switch (i) {
                    case 0:
                        return L"50$";
                    case 1:
                        return L"100$";
                    case 2:
                        return L"200$";
                    case 3:
                        return L"500$";
                    case 4:
                        return L"1000$";
                }
            } (i)
        );
    }

    m_leg_title_text.setFont(m_roboto_bold);
    m_leg_title_text.setCharacterSize(24);
    m_leg_title_text.setFillColor(sf::Color::White);
    m_leg_title_text.setString("Legenda");


    calc();
}

void GamePlay::events() {

}

void GamePlay::calc() {
    // PLAYER MOVE TEXTS POSITIONS AND CONTENT
    if (CGame::instance().m_round) {
        for (int i = 0; i < 4; i++) {
            m_playermove_text[i].setString((sf::String) CGame::instance().m_round->m_playermove_string[i]);
            m_playermove_text[i].setOrigin(m_playermove_text[i].getLocalBounds().width / 2, m_playermove_text[i].getLocalBounds().height / 2);
            if (i != 2)
                m_playermove_text[i].setRotation(i * 90);
            int pos_x = 0, pos_y = 0;
            switch (i) {
                case 0: {
                    pos_x = m_window->getSize().x / 2;
                    pos_y = m_window->getSize().y - 48 - 20 - 99 - 20 - 45 - 48;
                    break;
                }
                case 1: {
                    pos_x = 48 + 20 + 99 + 20 + 45;
                    pos_y = m_window->getSize().y / 2 - 24;
                    break;
                }
                case 2: {
                    pos_x = m_window->getSize().x / 2;
                    pos_y = 48 + 20 + 99 + 20 + 45;
                    break;
                }
                case 3: {
                    pos_x = m_window->getSize().x - 48 - 20 - 99 - 20 - 45;
                    pos_y = m_window->getSize().y / 2 - 24;
                    break;
                }
            }
            m_playermove_text[i].setPosition(pos_x, pos_y);
        }
    }


    // TABLE PLACEHOLDERS POSITIONS
    for (int i = 0; i < 5 * m_players_count; i++) {
        int pos_x = 0, pos_y = 0;
        if (i < 5) { // those are for player 0 (bottom)
            pos_x = m_window->getSize().x / 2 - 2.5 * 69 - 2 * 5 + (i % 5 * (69 + 5));
            pos_y = m_window->getSize().y - 99 - 20 - 48;
        }
        else if (i < 10) { // those are for player 1 (left)
            CGame::instance().m_table->m_card_placeholder[i].setSize(sf::Vector2f(99.f, 69.f));
            pos_x = 20;
            pos_y = m_window->getSize().y / 2 - 2.5 * 69 - 2 * 5 + (i % 5 * (69 + 5)) - 24;
        }
        else if (i < 15) { // ... (top)
            pos_x = m_window->getSize().x / 2 - 2.5 * 69 - 2 * 5 + (i % 5 * (69 + 5));
            pos_y = 20;
        }
        else if (i < 20) { // ... (right)
            CGame::instance().m_table->m_card_placeholder[i].setSize(sf::Vector2f(99.f, 69.f));
            pos_x = m_window->getSize().x - 20 - 99;
            pos_y = m_window->getSize().y / 2 - 2.5 * 69 - 2 * 5 + (i % 5 * (69 + 5)) - 24;
        }
        else { // too much players to drawing system
            CLog::instance() << "Table placeholder drawer has been bad initialized, too many players";
            break;
        }
        CGame::instance().m_table->m_card_placeholder[i].setPosition(pos_x, pos_y);
    }
    CGame::instance().m_table->m_cardstack_placeholder.setPosition(m_window->getSize().x / 3 * 2 - 69 / 2, m_window->getSize().y / 2 - 99 / 2 - 24);

    // CARDS POSITIONS AND TEXTURES
    for (int i = 0; i < m_players_count; i++) {
        for (int j = 0; j < CGame::instance().m_players[i]->m_cards.size(); j++) {
            int pos_x = 0, pos_y = 0;
            if (i == 0) { // those are for player 0 (bottom)
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setTexture(CGame::instance().m_players[i]->m_cards[j]->m_texture);
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setRotation(0.0f);
                pos_x = m_window->getSize().x / 2 - 2.5 * 69 - 2 * 5 + (j % 5 * (69 + 5));
                pos_y = m_window->getSize().y - 99 - 20 - 48;
            }
            else if (i == 1) { // those are for player 1 (left)
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setTexture(Card::m_texture_back);
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setRotation(-90.0f);
                pos_x = 20;
                pos_y = m_window->getSize().y / 2 - 2.5 * 69 - 2 * 5 + (j % 5 * (69 + 5)) + 69 - 24;
            }
            else if (i == 2) { // ... (top)
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setTexture(Card::m_texture_back);
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setRotation(0.0f);
                pos_x = m_window->getSize().x / 2 - 2.5 * 69 - 2 * 5 + (j % 5 * (69 + 5));
                pos_y = 20;
            }
            else if (i == 3) { // ... (right)
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setTexture(Card::m_texture_back);
                CGame::instance().m_players[i]->m_cards[j]->m_sprite.setRotation(90.0f);
                pos_x = m_window->getSize().x - 20;
                pos_y = m_window->getSize().y / 2 - 2.5 * 69 - 2 * 5 + (j % 5 * (69 + 5)) - 24;
            }
            else { // too much players to drawing system
                CLog::instance() << "Card drawer has been bad used, too many players";
            }
            CGame::instance().m_players[i]->m_cards[j]->m_sprite.setPosition(pos_x, pos_y);
        }
    }
    if (CGame::instance().m_table->m_cardstack.size() > 0) {
        CGame::instance().m_table->m_cardstack[0]->m_sprite.setTexture(Card::m_texture_back);
        CGame::instance().m_table->m_cardstack[0]->m_sprite.setRotation(0.0f);
        CGame::instance().m_table->m_cardstack[0]->m_sprite.setPosition(m_window->getSize().x / 3 * 2 - 69 / 2, m_window->getSize().y / 2 - 99 / 2 - 24);
    }

    // TOKENS AND THEIR COUNTERS POSITIONS
    for (int i = 0; i < m_players_count; i++) {
        int token_counter[5] = { 0, 0, 0, 0, 0 };
        for (int j = 0; j < CGame::instance().m_players[i]->m_tokens.size(); j++) {
            // first count tokens
            if (CGame::instance().m_players[i]->m_tokens[j]->m_type != Token::Type::DEALER) {
                token_counter[(
                    [] (Token::Type type)->int {
                        switch (type) {
                            case Token::Type::GREEN:
                                return 0;
                            case Token::Type::RED:
                                return 1;
                            case Token::Type::WHITE:
                                return 2;
                            case Token::Type::BLUE:
                                return 3;
                            case Token::Type::BLACK:
                                return 4;
                        }
                    } (CGame::instance().m_players[i]->m_tokens[j]->m_type)
                )]++;
            }
            
            int pos_x = 0, pos_y = 0;
            switch (i) { // which player?
                case 0: {
                    pos_x = m_window->getSize().x / 2 - CGame::instance().m_players[i]->m_tokens[j]->m_sprite.getGlobalBounds().width / 2 + 40 * 2 + 5 * 2;
                    pos_y = m_window->getSize().y - 20 - 99 - 20 - CGame::instance().m_players[i]->m_tokens[j]->m_sprite.getGlobalBounds().height - 20 - 48;
                    break;
                }
                case 1: {
                    pos_x = 20 + 99 + 20 + 20;
                    pos_y = m_window->getSize().y / 2 - CGame::instance().m_players[i]->m_tokens[j]->m_sprite.getGlobalBounds().height / 2 + 40 * 2 + 5 * 2 - 24;
                    break;
                }
                case 2: {
                    pos_x = m_window->getSize().x / 2 - CGame::instance().m_players[i]->m_tokens[j]->m_sprite.getGlobalBounds().width / 2 + 40 * 2 + 5 * 2 ;
                    pos_y = 20 + 99 + 20 + 20;
                    break;
                }
                case 3: {
                    pos_x = m_window->getSize().x - 20 - 99 - 20 - CGame::instance().m_players[i]->m_tokens[j]->m_sprite.getGlobalBounds().width - 20;
                    pos_y = m_window->getSize().y / 2 - CGame::instance().m_players[i]->m_tokens[j]->m_sprite.getGlobalBounds().height / 2 + 40 * 2 + 5 * 2 - 24;
                    break;
                }
                default: {
                    CLog::instance() << "Token drawer has been bad used, too many players";
                    break;
                }
            }
            int * pos = nullptr;
            if (i == 0 || i == 2) {
                pos = &pos_x;
            } else if (i == 1 || i == 3) {
                pos = &pos_y;
            }
            switch (CGame::instance().m_players[i]->m_tokens[j]->m_type) { // which token type? NO BREAKS HERE
                case Token::Type::GREEN:
                    *pos -= 45;
                case Token::Type::RED:
                    *pos -= 45;
                case Token::Type::WHITE:
                    *pos -= 45;
                case Token::Type::BLUE:
                    *pos -= 45;
                default:
                case Token::Type::BLACK: // empty here
                    break;
                case Token::Type::DEALER:
                    *pos += 45;
                    break;
            }
            CGame::instance().m_players[i]->m_tokens[j]->m_sprite.setPosition(pos_x, pos_y);
        }

        // set text for calculated tokens
        for (int j = 0; j < 5; j++) {
            m_token_text[i][j].setString((token_counter[j] == 0) ? "" : to_string(token_counter[j]));
        }

        // calculate position for token texts
        for (int j = 0; j < 5; j++) {
            m_token_text[i][j].setOrigin(m_token_text[i][j].getGlobalBounds().width / 2, m_token_text[i][j].getGlobalBounds().height / 2);
            switch (i) {
                case 0: {
                    m_token_text[i][j].setPosition(m_window->getSize().x / 2 - 45 * 2 + j * 45, m_window->getSize().y - 20 - 99 - 20 - m_token_text[i][j].getGlobalBounds().height / 2 - 48);
                    break;
                }
                case 1: {
                    m_token_text[i][j].setPosition(20 + 99 + 25, m_window->getSize().y / 2 - 45 * 2 + j * 45 - m_token_text[i][j].getGlobalBounds().height / 2 - 24);
                    break;
                }
                case 2: {
                    m_token_text[i][j].setPosition(m_window->getSize().x / 2 - 45 * 2 + j * 45, 20 + 99 + 25 - m_token_text[i][j].getGlobalBounds().height / 2);
                    break;
                }
                case 3: {
                    m_token_text[i][j].setPosition(m_window->getSize().x - 20 - 99 - 25, m_window->getSize().y / 2 - 45 * 2 + j * 45 -  m_token_text[i][j].getGlobalBounds().height / 2 - 24);
                    break;
                }
            }
        }

        // calculate token-on-table-value text position and text
        int value = calculateTokensValue(CGame::instance().m_table->m_tokens);
        m_tokenontablevalue_text.setString(to_string(value) + "$");
        m_tokenontablevalue_text.setPosition(m_window->getSize().x / 2 - m_tokenontablevalue_text.getGlobalBounds().width / 2, m_window->getSize().y / 2 - m_tokenontablevalue_text.getGlobalBounds().height / 2 - 24);
    }


    // LEGEND POSITIONS
    for (int i = 0; i < 5; i++) {
        m_leg_token_sprite[i].setOrigin(20, m_leg_token_sprite[i].getLocalBounds().height / 2);
        m_leg_token_sprite[i].setPosition(20, m_window->getSize().y - 48 - 20 - 25*i);
        m_leg_token_text[i].setOrigin(0, m_leg_token_text[i].getLocalBounds().height / 2);
        m_leg_token_text[i].setPosition(40, m_leg_token_sprite[i].getGlobalBounds().top + 8);
    }
    m_leg_title_text.setOrigin(20, m_leg_title_text.getLocalBounds().height / 2);
    m_leg_title_text.setPosition(40, m_leg_token_sprite[4].getGlobalBounds().top - 20);
}

void GamePlay::draw() {
    m_window->clear(sf::Color(0, 153, 0));
    
    // DRAW PLACEHOLDERS
    for (int i = 0; i < 20; i++) {
        m_window->draw(CGame::instance().m_table->m_card_placeholder[i]);
    }
    m_window->draw(CGame::instance().m_table->m_cardstack_placeholder);

    // DRAW CARDS
    for (int i = 0; i < m_players_count; i++) {
        for (int j = 0; j < CGame::instance().m_players[i]->m_cards.size(); j++) {
            m_window->draw(CGame::instance().m_players[i]->m_cards[j]->m_sprite);
        }
    }
    if (CGame::instance().m_table->m_cardstack.size() > 0)
        m_window->draw(CGame::instance().m_table->m_cardstack[0]->m_sprite);

    // TOKENS
    for (int i = 0; i < m_players_count; i++) {
        for (int j = 0; j < CGame::instance().m_players[i]->m_tokens.size(); j++) {
            m_window->draw(CGame::instance().m_players[i]->m_tokens[j]->m_sprite);
        }
    }

    // TOKEN TEXTS and PLAYER MOVE TEXTS
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            m_window->draw(m_token_text[i][j]);
        }

        if (i > 0)
            m_window->draw(m_playermove_text[i]);
    }
    m_window->draw(m_tokenontablevalue_text);

    // LEGEND OBJECTS
    m_window->draw(m_leg_title_text);
    for (int i = 0; i < 5; i++) {
        m_window->draw(m_leg_token_sprite[i]);
        m_window->draw(m_leg_token_text[i]);
    }
}
