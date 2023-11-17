#ifndef MINE_BUTTON_HPP
#define MINE_BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
#include <MINE/Animation.hpp>
#include <MINE/State.hpp>

#include <string>
#include <memory>
#include <functional>
#include <cmath>

namespace sf
{
    class Event;
    class RenderStates;
    class RenderTarget;
}

class Button : public sf::Drawable, public sf::Transformable
{
public:
    typedef std::shared_ptr<Button> Ptr;

    enum Type
    {
        Pressed,
        Selected,
        Normal,
        ButtonCount
    };

public:
    Button(State::Context context);

    void centerOrigin(sf::Sprite& sprite);
    void centerOrigin(sf::Text& text);
    sf::FloatRect getLocalBounds();

    void setText(const std::string &text, unsigned int size);
    void setPosition(float x, float y);
    void setTexture(Type type, const sf::Texture &texture);
    void setTexture(Type type, const sf::Texture &texture, const sf::IntRect &textureRect);

    void handleEvent(User user);
    void update(sf::Time dt);
    bool isPressed() const;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void changeTexture(Type type);

private:
    Animation mAnimation;
    sf::Sprite mSprite;
    sf::Sound mPressedSound;
    sf::Text mText;

    int width;
    int height;

    bool mIsHovered;
    bool mIsPressed;
    bool mPlayedPressedSound;
};

#endif // MINE_BUTTON_HPP
