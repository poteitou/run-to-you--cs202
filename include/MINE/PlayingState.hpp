#ifndef MINE_PLAYINGSTATE_HPP
#define MINE_PLAYINGSTATE_HPP


#include <SFML/Graphics.hpp>

#include <MINE/State.hpp>

#include <vector>
#include <string>
#include <iostream>

class PlayingState : public State
{
public:
    PlayingState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void render();

private:
    sf::Sprite mBackgroundSprite;
    sf::Sprite mGroundSprite;
    sf::FloatRect mViewBound;
    sf::View mView;
    float mScrollSpeed;
};

#endif // MINE_PLAYINGSTATE_HPP