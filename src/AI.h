#ifndef AI_H
#define AI_H

#include <ctime>
#include <vector>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Player.h"

class AI : public Player {
public:
    virtual Player::PossibleMoves auction(int value_to_beat, std::vector < Token * > & auction_return);
    virtual int cardChange(std::vector < Card * > & cardstack);
};

#endif
