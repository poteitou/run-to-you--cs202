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
#include <fstream>


namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class PlayingState : public State
{
public:
    PlayingState(StateStack &stack, Context context);

    virtual void createObstacle();
    virtual void recordScore();
    virtual void levelUp(sf::Time dt);
    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::array<sf::Sprite, 3> mBackgroundSprite;
    std::array<sf::Sprite, 3> mGroundSprite;
    std::array<std::string, 6> mTypeObject[3];
    std::deque<Object> mObstacleQueue;
    Animation mChange;
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
    int mLevel;
    bool mIsPaused;
    bool mStartLevelUp;
    bool mLevelingUp;
};

#endif // MINE_PLAYINGSTATE_HPP