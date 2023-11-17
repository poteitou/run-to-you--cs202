#ifndef USER_HPP
#define USER_HPP

#include <SFML/System/Vector2.hpp>

struct User
{
    User();

    sf::Vector2i mousePosition;
    bool isMousePressed;
    bool isMouseReleased;
    bool isEnterPressed;
    bool isSpacePressed;
    bool isEscapePressed;
    char keyPress;
};

#endif // USER_HPP