#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
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
        Normal,
        Selected,
        Pressed,
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
    bool isPressed() const;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void changeTexture(Type type);

private:
    sf::Sprite mSprite;
    // sf::Sound mSound;
    sf::Text mText;
    bool mIsHovered;
    bool mIsPressed;
};

#endif // BUTTON_HPP
