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
    enum ID {None, Intro, Menu, Instruct, Countdown, Begin, Middle, End, Paused, BTM, MTE, GameOver};
}

namespace Textures
{
    enum ID {MenuBG, PinkBG, OrangeBG, BlueBG, PinkToOrange, OrangeToBlue, Ground, GroundMenu, ToYou, PopupRect, Popup, Rank, CrushRight, CrushLeft, BlueSkirt, RainCoat, Life, Milktea, Dog, Tree, Rock, RockHeap, Bird, BirdGreen, Heart, Button, ButtonResume, ButtonReplay, ButtonQuit, ButtonPlay, ButtonMusic, ButtonMute, ButtonAbout, ButtonRank, ButtonX, ButtonSkip, ButtonEsc, ButtonUp, PinkMouse};
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
