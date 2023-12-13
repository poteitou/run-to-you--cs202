#ifndef MINE_PLAYINGSTATE_HPP
#define MINE_PLAYINGSTATE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <MINE/Collision.hpp>
#include <MINE/Player.hpp>
#include <MINE/Object.hpp>

#include <queue>
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
    enum Type
    {
        Milktea,
        Cat,
        Friend,
        Rock,
        Bird,
        Heart,
        TypeCount
    };

public:
    PlayingState(StateStack &stack, Context context);

    virtual void createObstacle();
    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::array<sf::Sprite, 3> mBackgroundSprite;
    std::array<sf::Sprite, 3> mGroundSprite;
    std::array<std::string, 6> mTypeObject;
    std::deque<Object> mObstacleQueue;
    sf::Sound mPaused;
    sf::Sound mGameOver;
    sf::Sprite mLives;
    Player mPlayer;
    sf::Text mDistanceText;
    float mGroundHeight;
    float mScrollSpeed;
    float mDistance;
    bool mIsPaused;
    int mCntLives;
};

#endif // MINE_PLAYINGSTATE_HPP