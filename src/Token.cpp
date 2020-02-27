#include "Token.h"

using namespace std;

Token::Token(Type type) {
    m_type = type;

    string filename = "token_";
    switch (m_type) {
        case Token::Type::DEALER: {
            filename += "dealer";
            break;
        }
        case Token::Type::GREEN: {
            filename += "green";
            break;
        }
        case Token::Type::RED: {
            filename += "red";
            break;
        }
        case Token::Type::WHITE: {
            filename += "white";
            break;
        }
        case Token::Type::BLUE: {
            filename += "blue";
            break;
        }
        case Token::Type::BLACK: {
            filename += "black";
            break;
        }
        default: {
            CLog::instance() << "Unspecified texture for this type of token";
            break;
        }
    }
    filename += ".png";

    if (!m_texture.loadFromFile("res/textures/" + filename)) {
        CLog::instance() << "Failed loading texture: " + filename;
    }
    
    m_sprite.setTexture(m_texture);
}
