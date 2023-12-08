#ifndef MINE_OBJECT_HPP
#define MINE_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
#include <MINE/Animation.hpp>
#include <MINE/Collision.hpp>
#include <MINE/Player.hpp>
#include <MINE/State.hpp>

namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class Object : public sf::Drawable, public sf::Transformable
{
public:
    typedef std::shared_ptr<Object> Ptr;
    
    enum Type
    {
        Milktea,
        Cat,
        Friend,
        Rock,
        Bird,
        Heart,
        TypeCount
    };

public:
    Object(State::Context context, Type type, float x, float y);

    void centerOrigin(sf::Sprite& sprite);
    sf::FloatRect getGlobalBounds();
    Type getType();

    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    
    bool isOutOfScreen();
    void changeTexture(sf::Texture &texture);

    void handleEvent(User user);
    void update(sf::Time dt, float scrollSpeed, float groundHeight);
    bool isCollide(const Player &player);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Animation mAnimation;
    sf::Sound mCollideSound;
    Type mType;

    int mWidth;
    int mHeight;

    bool mPlayedCollideSound;
    bool mIsCollide;
};

#endif // MINE_OBJECT_HPP