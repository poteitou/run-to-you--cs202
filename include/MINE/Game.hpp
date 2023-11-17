#ifndef MINE_GAME_HPP
#define MINE_GAME_HPP

#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
#include <MINE/StateStack.hpp>
#include <MINE/User.hpp>
// #include <Mine/State.hpp>
#include <Mine/IntroState.hpp>
#include <Mine/MenuState.hpp>
#include <Mine/PlayingState.hpp>
// #include <Mine/PauseState.hpp>
// #include <Mine/SettingsState.hpp>
// #include <Mine/GameOverState.hpp>

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
    SoundBufferHolder mSoundBuffers;
    TextureHolder mTextures;
    FontHolder mFonts;
    User mUser;

    StateStack mStateStack;

    std::size_t mStatisticsNumFrames;
    sf::Time mStatisticsUpdateTime;
    sf::Text mStatisticsText;
    sf::Sprite mMouseSprite;
};

#endif // MINE_GAME_HPP
