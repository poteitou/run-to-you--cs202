#ifndef MINE_INTROSTATE_HPP
#define MINE_INTROSTATE_HPP

#include <SFML/Graphics.hpp>

#include <MINE/State.hpp>
// #include <MINE/Button.hpp>

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

#endif // MINE_INTROSTATE_HPP