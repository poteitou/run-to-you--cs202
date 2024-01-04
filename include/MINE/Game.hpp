#ifndef MINE_GAME_HPP
#define MINE_GAME_HPP

#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
#include <MINE/StateStack.hpp>
#include <MINE/Collision.hpp>
#include <MINE/User.hpp>
#include <Mine/IntroState.hpp>
#include <Mine/MenuState.hpp>
#include <Mine/BeginState.hpp>
#include <Mine/PausedState.hpp>
#include <Mine/GameOverState.hpp>
#include <Mine/BTMState.hpp>
#include <Mine/Countdown.hpp>

#include <string.h>

class Game
{
public:
    Game();
    void run();

private:
    void processInput();
    void update(sf::Time dt);
    void render();

    void updateStatistics(sf::Time dt);
    void registerStates();

private:
    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow;
    sf::Cursor mCursor;
    sf::Image mCursorImage;
    SoundBufferHolder mSoundBuffers;
    TextureHolder mTextures;
    FontHolder mFonts;
    User mUser;

    StateStack mStateStack;
    bool mIsPaused;

    std::size_t mStatisticsNumFrames;
    sf::Time mStatisticsUpdateTime;
    sf::Text mStatisticsText;
};

#endif // MINE_GAME_HPP
