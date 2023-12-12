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

#include <string>

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
    Object(State::Context context, std::string type, float x, float y);

    void centerOrigin(sf::Sprite& sprite);
    sf::FloatRect getGlobalBounds();
    std::string getType();

    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    
    bool isOutOfScreen();
    void changeTexture(sf::Texture &texture);

    void handleEvent(User user);
    void update(sf::Time dt, float scrollSpeed, const Player &player);
    bool isCollide();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Animation mAnimation;
    sf::Sound mCollideSound;
    std::string mType;
    sf::Text mNotification;

    int mWidth;
    int mHeight;

    bool mPlayedCollideSound;
    bool mIsCollide;
    bool mAlreadyCollide;
};

#endif // MINE_OBJECT_HPP