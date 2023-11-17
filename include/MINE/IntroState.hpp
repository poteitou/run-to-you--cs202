#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <MINE/State.hpp>
// #include <MINE/Button.hpp>
// #include <MINE/Label.hpp>

class IntroState : public State
{
public:
    IntroState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void render();

private:
    sf::Sprite mBackgroundSprite;
    sf::Text mText;
    bool mShowText;
    sf::Time mTextEffectTime;
};

#endif // INTROSTATE_HPP