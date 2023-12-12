#ifndef MINE_PAUSEDSTATE_HPP
#define MINE_PAUSEDSTATE_HPP

#include <SFML/Graphics.hpp>

#include <MINE/State.hpp>
#include <MINE/Button.hpp>

#include <array>

namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class PausedState : public State
{
public:
    PausedState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::RectangleShape mBackgroundRect;
    std::array<Button::Ptr, 2> mButtons;
    sf::Text mTitle;
};

#endif // MINE_PAUSEDSTATE_HPP
