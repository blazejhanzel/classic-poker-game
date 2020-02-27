#ifndef ROUND_H
#define ROUND_H

#include <vector>
#include <algorithm>
#include <string>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

#include "CLog.h"
#include "Table.h"
#include "Player.h"
#include "Card.h"
#include "Sequence.h"

class Round {
    friend class GamePlay;
    
    Table * m_table;
    std::vector < Player * > m_players;
    std::vector < Card * > m_deck;
    std::vector < Sequence * > m_sequences;
    std::string m_playermove_string[4] = { "", "Czeka", "Czeka", "Czeka" };

    void wait(int ms);

public:
    Round(Table * table, std::vector < Player * > players, std::vector < Card * > deck);
    ~Round();
    void begin();
};

#endif
