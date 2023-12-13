#ifndef MINE_INTROSTATE_HPP
#define MINE_INTROSTATE_HPP

#include <SFML/Graphics.hpp>

#include <MINE/Animation.hpp>
#include <MINE/State.hpp>

class IntroState : public State
{
public:
    IntroState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mBackgroundSprite;
    sf::Text mTitle;
    sf::Sprite mTitleSprite;
    Animation mGirl;
    sf::Text mText;
    bool mShowText;
    sf::Time mTextEffectTime;
};

#endif // MINE_INTROSTATE_HPP