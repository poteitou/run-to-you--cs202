#ifndef MINE_BEGINSTATE_HPP
#define MINE_BEGINSTATE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <MINE/Collision.hpp>
#include <MINE/Player.hpp>
#include <MINE/Object.hpp>

#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <queue>

namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class BeginState : public State
{
public:
    BeginState(StateStack &stack, Context context);

    int randomInt(int l, int r);
    virtual void createObstacle();
    virtual void recordScore();
    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::array<sf::Sprite, 3> mBackgroundSprite;
    std::array<sf::Sprite, 3> mGroundSprite;
    std::vector<std::string> mTypeObject;
    std::deque<Object> mObstacleQueue;
    Animation mCrush;
    sf::Sound mCollide;
    sf::Sound mPaused;
    sf::Sound mGameOver;
    sf::Music mMusic;
    sf::Sprite mLives;
    Player mPlayer;
    float mTimeCollide;
    sf::Text mDistanceText;
    float mGroundHeight;
    float mScrollSpeed;
    float mDistance;
    int mCntLives;
    bool mIsPaused;
};

#endif // MINE_BEGINSTATE_HPP