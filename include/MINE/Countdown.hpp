#ifndef MINE_COUNTDOWN_HPP
#define MINE_COUNTDOWN_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <MINE/Animation.hpp>
#include <MINE/State.hpp>

class Countdown : public State
{
public:
    Countdown(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::RectangleShape mBackgroundRect;
    sf::Text mTitle;
    // sf::Sound mFirstSound;
    // sf::Music mMusic;
    sf::Text mText;
    bool mShowText;
    float mTime;
};

#endif // MINE_COUNTDOWN_HPP