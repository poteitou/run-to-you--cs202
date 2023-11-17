#ifndef MINE_MENUSTATE_HPP
#define MINE_MENUSTATE_HPP

#include <SFML/Graphics.hpp>

#include <MINE/State.hpp>
#include <MINE/Button.hpp>

#include <array>

class MenuState : public State
{
public:
    MenuState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void render();

private:
    sf::Sprite mBackgroundSprite;
    std::array<Button::Ptr, 2> mButtons;
};

#endif // MINE_MENUSTATE_HPP
