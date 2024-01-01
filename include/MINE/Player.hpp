#ifndef MINE_PLAYER_HPP
#define MINE_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <MINE/ResourceHolder.hpp>
#include <MINE/Identifier.hpp>
#include <MINE/Animation.hpp>
#include <MINE/State.hpp>

namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class Player : public sf::Drawable, public sf::Transformable
{
public:
    typedef std::shared_ptr<Player> Ptr;
    
    enum Type
    {
        BlueSkirt,
        RainCoat,
        TypeCount
    };

public:
    Player(State::Context context);

    void centerBottom(sf::Sprite& sprite);
    sf::FloatRect getGlobalBounds();
    void setFrame(int frame);
    void setVelocity(sf::Vector2f velocity);
    void setVelocity(float vx, float vy);
    void accelerate(sf::Vector2f velocity);
    void accelerate(float vx, float vy);
    sf::Vector2f getVelocity() const;
    const sf::Sprite& getSprite() const;
    
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void changeTexture(sf::Texture &texture1, sf::Texture &texture2);

    void handleEvent(User user);
    void update(sf::Time dt, float groundHeight);
    bool isJumping() const;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Animation mAnimation;
    sf::Sprite mSprite;
    sf::Vector2f mVelocity;
    sf::Vector2f mPosition;
    float mGravity;

    int mWidth;
    int mHeight;

    bool mAlreadyJump;
    bool mIsRunning;
    int mIsJumping;
};

#endif // MINE_PLAYER_HPP