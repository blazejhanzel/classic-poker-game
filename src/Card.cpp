#include "Card.h"

using namespace std;

Card::Card(int power, int color) {
    m_power = (Power)power;
    m_color = (Color)color;

    string filename_prefix;
    switch (this->m_power) {
        case Card::Power::_J: {
            filename_prefix = "jack";
            break;
        }
        case Card::Power::_Q: {
            filename_prefix = "queen";
            break;
        }
        case Card::Power::_K: {
            filename_prefix = "king";
            break;
        }
        case Card::Power::_A: {
            filename_prefix = "1";
            break;
        }
        default: {
            filename_prefix = to_string((int) m_power + 2);
            break;
        }
    }

    string filename_suffix;
    switch (this->m_color) {
        case Card::Color::PIK: {
            filename_suffix = "spade";
            break;
        }
        case Card::Color::KIER: {
            filename_suffix = "heart";
            break;
        }
        case Card::Color::KARO: {
            filename_suffix = "diamond";
            break;
        }
        case Card::Color::TREFL: {
            filename_suffix = "club";
            break;
        }
        default: break;
    }

    string filename = filename_prefix + "_" + filename_suffix + ".png";

    if (!this->m_texture.loadFromFile("res/textures/" + filename)) {
        CLog::instance() << "Failed loading texture: " + filename;
    }

    if (!Card::m_texture_back.loadFromFile("res/textures/back.png")) {
        CLog::instance() << "Failed loading texture: back.png";
    }
    
    m_sprite.setTexture(m_texture);
}
