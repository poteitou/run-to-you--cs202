#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>  
#include <MINE/User.hpp>
#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
#include <MINE/StateStack.hpp>

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
    TextureHolder mTextures;
    FontHolder mFonts;
    User mUser;

    StateStack mStateStack;

    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
};

#endif // GAME_HPP
