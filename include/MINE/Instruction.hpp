#ifndef MINE_INSTRUCTION_HPP
#define MINE_INSTRUCTION_HPP

#include <SFML/Graphics.hpp>

#include <MINE/State.hpp>
#include <MINE/Button.hpp>

#include <array>

namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class Instruction : public State
{
public:
    Instruction(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::RectangleShape mBackgroundRect;
    Animation mEsc, mUp;
    std::vector<sf::Text> mText;
    Button::Ptr mButton;
};

#endif // MINE_INSTRUCTION_HPP
