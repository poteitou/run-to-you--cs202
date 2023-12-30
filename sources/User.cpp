#include <MINE/User.hpp>

User::User()
: mousePosition(0, 0)
, isMouseWheelUp(false)
, isMouseWheelDown(false)
, isMousePressed(false)
, isMouseReleased(false)
, isEnterPressed(false)
, isSpacePressed(false)
, isEscapePressed(false)
, isUpPressed(false)
, keyPress('$')
{
}