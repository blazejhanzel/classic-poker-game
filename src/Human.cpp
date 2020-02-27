#include "Human.h"

using namespace std;

Human::Human(sf::RenderWindow & window) : UI(window) {
    // LOAD FONTS
    if (!m_roboto_regular.loadFromFile("res/fonts/Roboto-Regular.ttf"))
        CLog::instance() << "Error loading font";
    if (!m_roboto_bold.loadFromFile("res/fonts/Roboto-Bold.ttf"))
        CLog::instance() << "Error loading font";


    // INITIALIZE BACKGROUND
    m_background.setSize(sf::Vector2f(m_window->getSize().x, 44.f));
    m_background.setFillColor(sf::Color(0, 0, 0, 128));
    m_background.setOutlineThickness(4);
    m_background.setOutlineColor(sf::Color(0, 0, 0, 255 * 0.75));
    m_background.setPosition(0, m_window->getSize().y - 44);


    // INITIALIZE TEXTS
    m_info_text.setFont(m_roboto_bold);
    m_fold_text.setFont(m_roboto_bold);
    m_action_text.setFont(m_roboto_bold);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            m_auctionctrl_text[i][j].setFont(m_roboto_bold);

    m_info_text.setCharacterSize(18);
    m_fold_text.setCharacterSize(18);
    m_action_text.setCharacterSize(18);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            m_auctionctrl_text[i][j].setCharacterSize((j == 2) ? 32 : 48);

    m_info_text.setFillColor(sf::Color::White);
    m_fold_text.setFillColor(sf::Color::White);
    m_action_text.setFillColor(sf::Color::White);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            m_auctionctrl_text[i][j].setFillColor(sf::Color::White);

    m_info_text.setString(L"Czekasz. Pozostali gracze wykonują ruch...");
    m_fold_text.setString(L"Spasuj");
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 2; j++)
            m_auctionctrl_text[i][j].setString((j == 1) ? "+" : "-");
    
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 2; j++)
            m_auctionctrl_text[i][j].setOrigin(m_auctionctrl_text[i][j].getGlobalBounds().width / 2, m_auctionctrl_text[i][j].getGlobalBounds().height / 2);


    calc();
}

enum Player::PossibleMoves Human::auction(int value_to_beat, std::vector < Token * > & auction_return) {
    m_state = AUCTION;
    m_move = UNDEFINED;
    m_auction_return = &auction_return;
    m_valuetobeat = value_to_beat;
    for (int i = 0; i < 5; i++)
        m_auctionctrl_counter[i] = 0;

    while (m_state != WAITING) {
        Sleep(500); // prevent CPU from unwanted calculations but feels like quick lag
    }

    if (calculateTokensValue(m_tokens) == 0)
        return Player::PossibleMoves::ALLIN;
    else if (m_move == FOLD)
        return Player::PossibleMoves::FOLD;
    else {
        return [] (int retvalue, int tobeat)->Player::PossibleMoves {
            if (retvalue > tobeat)
                return PossibleMoves::RAISE;
            else if (retvalue == tobeat)
                return PossibleMoves::CALL;
        } (calculateTokensValue(*m_auction_return), value_to_beat);
    }
}

int Human::cardChange(std::vector < Card * > & cardstack) {
    m_state = CARDCHANGE;

    while (m_state != WAITING) {
        Sleep(500); // prevent CPU from unwanted calculations but feels like quick lag
    }

    int count = 0;
    for (int i = 4; i >= 0; i--) {
        if (m_cardchange[i]) {
            m_cards[i]->m_sprite.setColor(sf::Color::White);
            cardstack.push_back(m_cards[i]);
            m_cards[i] = cardstack[0];
            cardstack.erase(cardstack.begin());
            m_cardchange[i] = false;
            count++;
        }
    }

    return count;
}

void Human::events() {
    // fold text and events
    if (m_state == AUCTION) {
        if (m_fold_bound.contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
            m_fold_text.setFillColor(sf::Color::Yellow);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_move = FOLD;
                m_info_text.setString(L"Spasowałeś. Poczekaj do końca rundy.");
                m_state = WAITING;
            }
        } else {
            m_fold_text.setFillColor(sf::Color::White);
        }
    }

    // action text and events
    if (m_state == AUCTION || m_state == CARDCHANGE) {
        if (m_action_bound.contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
            m_action_text.setFillColor(sf::Color::Yellow);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    Sleep(250);
                }
                switch (m_state) {
                    case AUCTION: {
                        bool allin = (m_auctionctrl_counter[0] == m_token_counter[0] && m_auctionctrl_counter[1] == m_token_counter[1] && m_auctionctrl_counter[2] == m_token_counter[2] && m_auctionctrl_counter[3] == m_token_counter[3] && m_auctionctrl_counter[4] == m_token_counter[4]) ? true : false;
                        int suma = m_auctionctrl_counter[0] * 50 + m_auctionctrl_counter[1] * 100 + m_auctionctrl_counter[2] * 200 + m_auctionctrl_counter[3] * 500 + m_auctionctrl_counter[4] * 1000; // TODO: Change this for Token::Type values
                        if (suma >= m_valuetobeat || allin) {
                            for (int i = m_tokens.size() - 1; i >= 0; i--) {
                                if (m_tokens[i]->m_type != Token::Type::DEALER) {
                                    int j = 0;
                                    switch (m_tokens[i]->m_type) {
                                        case Token::Type::RED: {
                                            j = 1;
                                            break;
                                        }
                                        case Token::Type::WHITE: {
                                            j = 2;
                                            break;
                                        }
                                        case Token::Type::BLUE: {
                                            j = 3;
                                            break;
                                        }
                                        case Token::Type::BLACK: {
                                            j = 4;
                                            break;
                                        }
                                    }
                                    if (m_auctionctrl_counter[j] > 0) {
                                        m_auctionctrl_counter[j]--;
                                        m_auction_return->push_back(m_tokens[i]);
                                        m_tokens.erase(m_tokens.begin() + i);
                                    }
                                }
                            }
                            if (allin)
                                m_info_text.setString(L"Wchodzisz all-in z " + (sf::String) to_string(suma) + "$");
                            else if (suma == m_valuetobeat)
                                m_info_text.setString(L"Sprawdzasz. Poczekaj aż przeciwnicy skończą licytację.");
                            else if (suma > m_valuetobeat)
                                m_info_text.setString(L"Podbijasz do " + (sf::String) to_string(suma) + "$");
                            m_state = WAITING;
                        } else
                            m_info_text.setString(L"Musisz zalicytować przynajmniej " + (sf::String)to_string(m_valuetobeat) + "$");
                        break;
                    }
                    case CARDCHANGE: {
                        m_info_text.setString(L"Poczekaj na ruch przeciwników.");
                        m_state = WAITING;
                        break;
                    }
                    default: {
                        CLog::instance() << "An error occured in Human";
                        break;
                    }
                }
            }
        } else {
            m_action_text.setFillColor(sf::Color::White);
        }
    }

    // cards texts and events
    if (m_state == CARDCHANGE) {
        if (m_cards.size() == 5) {
            for (int i = 0; i < 5; i++) {
                if (m_card_bound[i].contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
                    m_cards[i]->m_sprite.setColor(sf::Color::Yellow);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        m_cardchange[i] = (m_cardchange[i]) ? false : true;
                        while(sf::Mouse::isButtonPressed(sf::Mouse::Left));
                    }
                } else {
                    m_cards[i]->m_sprite.setColor((m_cardchange[i]) ? sf::Color::Yellow : sf::Color::White);
                }
            }
        }
    }

    // auctioncontrol texts and events
    if (m_state == AUCTION) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                if (m_auctionctrl_bound[i][j].contains((sf::Vector2f) sf::Mouse::getPosition(*m_window))) {
                    m_auctionctrl_text[i][j].setFillColor(sf::Color::Yellow);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if (j == 0) {
                            if (m_auctionctrl_counter[i] - 1 >= 0) {
                                m_auctionctrl_counter[i]--;
                            }
                        }
                        else /* if (j == 1) */ {
                            if (m_auctionctrl_counter[i] + 1 <= m_token_counter[i]) {
                                m_auctionctrl_counter[i]++;
                            }
                        }
                        while (sf::Mouse::isButtonPressed(sf::Mouse::Left));
                    }
                } else {
                    m_auctionctrl_text[i][j].setFillColor(sf::Color::White);
                }
            }
        }
    }
}

void Human::calc() {
    // CALCULATES DEPENDING ON STATE
    switch (m_state) {
        default:
        case WAITING: {
            // empty now
            break;
        }
        case AUCTION: {
            m_info_text.setString(L"Licytujesz. Co chcesz zrobić?");
            m_action_text.setString(L"Postaw");
            for (int i = 0; i < 5; i++)
                m_auctionctrl_text[i][2].setString((sf::String) to_string(m_auctionctrl_counter[i]));

            // CALCULATING HOW MUCH TOKENS OF EVERY TYPE PLAYER HAVE
            for (int i = 0; i < 5; i++)
                m_token_counter[i] = 0;
            for (int j = 0; j < m_tokens.size(); j++) {
                // first count tokens
                if (m_tokens[j]->m_type != Token::Type::DEALER) {
                    m_token_counter[(
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
                        } (m_tokens[j]->m_type)
                    )]++;
                }
            }
            break;
        }
        case CARDCHANGE: {
            m_info_text.setString(L"Wymieniasz. Wybierz karty i kliknij \"wymień\".");
            m_action_text.setString(L"Wymień");
            break;
        }
    }

    // SET POSITIONS
    m_info_text.setPosition(10, m_window->getSize().y - 18 - 15);
    m_fold_text.setPosition(m_window->getSize().x - 10 - 10 - m_action_text.getGlobalBounds().width - m_fold_text.getGlobalBounds().width, m_window->getSize().y - 18 - 15);
    m_action_text.setPosition(m_window->getSize().x - 10 - m_action_text.getGlobalBounds().width, m_window->getSize().y - 18 - 15);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++) {
            if (j == 2)
                m_auctionctrl_text[i][j].setOrigin(m_auctionctrl_text[i][j].getGlobalBounds().width / 2, m_auctionctrl_text[i][j].getGlobalBounds().height / 2);
            m_auctionctrl_text[i][j].setPosition(m_window->getSize().x / 2 - 45 * 2 + i * 45, m_window->getSize().y - 48 - 20 - 99 - 48 - 40 - 40 - j * 24);
        }

    // SET BOUNDING BOXES
    m_fold_bound = m_fold_text.getGlobalBounds();
    m_action_bound = m_action_text.getGlobalBounds();
    if (m_cards.size() == 5)
        for (int i = 0; i < 5; i++)
            m_card_bound[i] = m_cards[i]->m_sprite.getGlobalBounds();
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 2; j++) {
            m_auctionctrl_bound[i][j] = m_auctionctrl_text[i][j].getGlobalBounds();
            m_auctionctrl_bound[i][j].left -= 9;
            m_auctionctrl_bound[i][j].top -= 9;
            m_auctionctrl_bound[i][j].width += 18;
            m_auctionctrl_bound[i][j].height += 18;
        }
}

void Human::draw() {
    m_window->draw(m_background);
    m_window->draw(m_info_text);

    switch (m_state) {
        default:
        case WAITING: {
            // empty, nothing to draw
            break;
        }
        case AUCTION: {
            m_window->draw(m_fold_text);
            m_window->draw(m_action_text);
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 2; j++)
                    m_window->draw(m_auctionctrl_text[i][j]);
                if (m_auctionctrl_text[i][2].getString() != (sf::String) "0")
                    m_window->draw(m_auctionctrl_text[i][2]);
            }
            break;
        }
        case CARDCHANGE: {
            m_window->draw(m_action_text);
            break;
        }
    }
}
