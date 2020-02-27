#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <algorithm>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

#include "CLog.h"
#include "Card.h"
#include "Token.h"

class Player {
    friend class Round;
    friend class CUI;
    friend class GamePlay;

protected:
    enum PossibleMoves {
        FOLD, // pas // resign in round
        CALL, // sprawdzenie // put minimal amount of money
        RAISE, // podbicie // put more than minimal amount of money
        ALLIN // all-in // get all of money to auction
    };

    std::vector < Card * > m_cards;
    std::vector < Token * > m_tokens;

public:
    // App functions
    // virtual std::function < bool() > move() = 0; // player move, AI will "think" what to do,Humna need to be asked by UI
    void getCard(Card * card); // get card from givver (e.g. Table giving cards to player) and put them into m_cards
    void getTokens(std::vector < Token * > tokens); // get tokens from giver and put them into m_tokens
    bool isDealer(); // checks if player is a dealer, so if it have dealers token

    // Poker play methods
    std::vector < Token * > ante(); // "ante", returns token that player need to put in round
    virtual enum PossibleMoves auction(int value_to_beat, std::vector < Token * > & auction_return) = 0; // this is first auction in game

    // mixed Poker play and app functions
    virtual int cardChange(std::vector < Card * > & cardstack) = 0; // change cards, returns count of changed cards
};

#endif
