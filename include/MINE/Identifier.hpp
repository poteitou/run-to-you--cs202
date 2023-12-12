#ifndef MINE_IDENTIFIER_HPP
#define MINE_IDENTIFIER_HPP

#include <MINE/ResourceHolder.hpp>

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
}

namespace States
{
    enum ID {None, Intro, Menu, Setting, Playing, Paused, EndLevel, GameOver, HighScore, About};
}

namespace Textures
{
    enum ID {PinkBackground, Ground, BlueSkirt, RainCoat, Life, Milktea, Cat, Friend, Rock, Bird, Heart, Button, PinkMouse};
}

namespace Fonts
{
    enum ID {Title, Main};
}

namespace Sounds
{
    enum ID {Main, Button, Jump, Heart};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundBufferHolder;

#endif // MINE_IDENTIFIER_HPP
