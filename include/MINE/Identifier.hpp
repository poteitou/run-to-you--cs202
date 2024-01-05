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
    enum ID {None, Intro, Menu, Countdown, Setting, Begin, Middle, End, Paused, BTM, MTE, GameOver, HighScore};
}

namespace Textures
{
    enum ID {MenuBG, PinkBG, OrangeBG, BlueBG, PinkToOrange, OrangeToBlue, Ground, ToYou, PopupRect, CrushRight, BlueSkirt, BlueCollide, RainCoat, Life, Milktea, Dog, Tree, Rock, Bird, Heart, Button, ButtonPlay, ButtonMusic, ButtonMute, ButtonAbout, ButtonRank, ButtonX, ButtonSkip, PinkMouse};
}

namespace Fonts
{
    enum ID {Title, Caro, Main};
}

namespace Sounds
{
    enum ID {First, Button, Collide, Paused, GameOver, Heart, Countdown};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundBufferHolder;

#endif // MINE_IDENTIFIER_HPP
