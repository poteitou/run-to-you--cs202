#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

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

#endif // MENUSTATE_HPP