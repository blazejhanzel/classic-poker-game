#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "UI/UI.h"

// multiple inheritance is a good step here, because Human is kind of Player,
// but need interfeace to communicate with user
class Human : public Player, public UI {
    enum UIState {
        WAITING, // no human move, player waiting
        AUCTION, // human need to specife tokens that want to auction
        CARDCHANGE // human need to specife cards to change
    } m_state = WAITING;

    sf::Font m_roboto_regular, m_roboto_bold;
    sf::RectangleShape m_background;
    sf::Text m_info_text, m_fold_text, m_action_text, m_auctionctrl_text[5][3];
    sf::FloatRect m_fold_bound, m_action_bound, m_card_bound[5], m_auctionctrl_bound[5][2];
    int m_auctionctrl_counter[5] = { 0, 0, 0, 0, 0 };
    int m_token_counter[5] = { 0, 0, 0, 0, 0 };

    enum HumanPossibleMoves { // temporary or not, maybe can use Player::PossibleMoves instead of this
        UNDEFINED,
        FOLD,
        ALLIN
    } m_move;
    std::vector < Token * > * m_auction_return;
    int m_valuetobeat = 0;

    bool m_cardchange[5] = { false, false, false, false, false };

public:
    Human(sf::RenderWindow & window);

    virtual enum Player::PossibleMoves auction(int value_to_beat, std::vector < Token * > & auction_return);
    virtual int cardChange(std::vector < Card * > & cardstack);

    virtual void events();
    virtual void calc();
    virtual void draw();
};

#endif
