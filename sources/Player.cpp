#include <MINE/Player.hpp>

Player::Player(State::Context context, bool isPlayer)
    : mAnimation(),
      mVelocity(0.f, 0.f),
      mIsRunning(true),
      mAlreadyJump(false),
      mIsJumping(0),
      mGravity(1875.f)
{
    // top, left, width, height
    if (isPlayer)
    {
        mAnimation.setTexture(context.mTextures->get(Textures::BlueSkirt));
        mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
        mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 3;
    }
    else
    {
        mAnimation.setTexture(context.mTextures->get(Textures::CrushLeft));
        mWidth = context.mTextures->get(Textures::CrushLeft).getSize().x / 8;
        mHeight = context.mTextures->get(Textures::CrushLeft).getSize().y;
    }

    mAnimation.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mAnimation.setNumFrames(8);
    mAnimation.setDuration(sf::seconds(1.f));
    mAnimation.centerBottom();
    mAnimation.setRepeating(true);
}

void Player::centerBottom(sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height));
}

sf::FloatRect Player::getGlobalBounds()
{
    return mAnimation.getGlobalBounds();
}

void Player::setFrame(int frame)
{
    mAnimation.setFrame(frame);
}

void Player::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Player::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

void Player::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

void Player::accelerate(float vx, float vy)
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

sf::Vector2f Player::getVelocity() const
{
    return mVelocity;
}

const sf::Sprite &Player::getSprite() const
{
    return mAnimation.getSprite();
}

void Player::setPosition(float x, float y)
{
    mPosition.x = x;
    mPosition.y = y;
    mAnimation.setPosition(x, y);
}

sf::Vector2f Player::getPosition() const
{
    return mPosition;
}

void Player::changeTexture(sf::Texture &texture)
{
    mAnimation.setTexture(texture);
}

void Player::handleEvent(User user)
{
    if (user.isSpacePressed)
    {
        if (!mAlreadyJump && mIsJumping <= 1)
        {
            mVelocity.y = -750.f;
            mAlreadyJump = true;
            ++mIsJumping;
        }
    }
    else 
    {
        mAlreadyJump = false;
    }
}

void Player::update(sf::Time dt, float groundHeight)
{
    if (groundHeight == -1.f)
    {
        mAnimation.setFrame(0);
        return;
    }
    if (groundHeight == 0.f)
    {
        mAnimation.setFrame(8);
        return;
    }

    mPosition.y += mVelocity.y * dt.asSeconds();
    mPosition.x += mVelocity.x * dt.asSeconds();
    mAnimation.setPosition(mPosition);

    // jumping mechanic
    if (mPosition.y < groundHeight) // above ground
    {
        mVelocity.y += mGravity * dt.asSeconds(); // add gravity
        // mIsJumping = true;
    }
    else if (mPosition.y > groundHeight) // below ground
    {
        mPosition.y = groundHeight;
    }

    if (mPosition.y == groundHeight)
    {
        mIsJumping = 0;
        mVelocity.y = 0.f;
    }

    if (mIsJumping > 0)
        mAnimation.setFrame(0);
    else 
        mAnimation.update(dt);
}

bool Player::isJumping() const
{
    return mIsJumping > 0;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mAnimation, states);
}

