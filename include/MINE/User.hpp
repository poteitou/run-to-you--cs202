#ifndef MINE_USER_HPP
#define MINE_USER_HPP

#include <SFML/System.hpp>

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

#endif // MINE_USER_HPP