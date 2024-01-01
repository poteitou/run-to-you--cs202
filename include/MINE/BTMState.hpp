#ifndef MINE_BTMSTATE_HPP
#define MINE_BTMSTATE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <MINE/Animation.hpp>
#include <MINE/State.hpp>

class BTMState : public State
{
public:
    BTMState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mBackgroundSprite;
    sf::Sprite mGroundSprite;
    sf::Sound mTextSound;
    Animation mGirl;
    Animation mCrush;
    Animation mChange;
    sf::Sprite mLives;
    sf::Music mMusic;
    std::array<sf::RectangleShape, 4> mRect;
    sf::Text mDistanceText;
    std::array<sf::Text, 2> mText;
    int mShowText;
    float mTime;
};

#endif // MINE_BTMSTATE_HPP