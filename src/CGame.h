#ifndef CGAME_H
#define CGAME_H

#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <cstdlib>

#include "CLog.h"
#include "Player.h"
#include "Human.h"
#include "AI.h"
#include "Token.h"
#include "Card.h"
#include "Table.h"
#include "Round.h"

class CGame {
    friend class CUI;
    friend class GamePlay;
    friend class GameOver;

private:
    std::vector < Player * > m_players;
    std::vector < Token * > m_tokens;
    std::vector < Card * > m_deck;
    Table * m_table = nullptr;
    Round * m_round;
    bool m_end_signal = false;

    CGame();
    ~CGame();
    CGame(const CGame &);

public:
    inline static enum GameState {
        MAIN_MENU,
        GAMEPLAY,
        GAMEOVER
    } m_gamestate = MAIN_MENU;

    static CGame & instance() {
        static CGame singleton;
        return singleton;
    }
    void start(Human * human, int players = 4);
    void end();
};

#endif
