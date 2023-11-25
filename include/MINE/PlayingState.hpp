#ifndef MINE_PLAYINGSTATE_HPP
#define MINE_PLAYINGSTATE_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <MINE/State.hpp>
#include <MINE/Player.hpp>

#include <vector>
#include <string>
#include <iostream>


namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class PlayingState : public State
{
public:
    PlayingState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::array<sf::Sprite, 3> mBackgroundSprite;
    std::array<sf::Sprite, 3> mGroundSprite;
    Player mPlayer;
    sf::Text mDistanceText;
    float mGroundHeight;
    float mScrollSpeed;
    float mDistance;
    bool mIsPaused;
};

#endif // MINE_PLAYINGSTATE_HPP