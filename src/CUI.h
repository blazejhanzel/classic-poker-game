#ifndef CUI_H
#define CUI_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

#include "CGame.h"
#include "CLog.h"
#include "UI/UI.h"
#include "UI/MainMenu.h"
#include "UI/GamePlay.h"
#include "UI/LoadingScreen.h"
#include "UI/GameOver.h"
#include "Human.h"

class CUI {
private:
    inline static sf::RenderWindow m_window;
    inline static sf::RenderWindow m_windev;

    inline static sf::Font m_roboto_regular;
    inline static UI * m_mainmenu, * m_gameplay, * m_gameover;

    inline static bool m_gameplay_loaded = false;
    inline static Human * m_human;

    CUI();
    ~CUI();
    CUI(const CUI &);

    static void startLogic();

    static void windowRender();
    static void windevRender();

public:
    static CUI & instance() {
        static CUI singleton;
        return singleton;
    }

    static void start();
};

#endif
