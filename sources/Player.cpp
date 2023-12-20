#include <MINE/Player.hpp>

Player::Player(State::Context context)
    : mAnimation(),
      mSprite(context.mTextures->get(Textures::BlueCollide)),
      mJumpSound(context.mSoundBuffers->get(Sounds::Jump)),
      mVelocity(0.f, 0.f),
      mIsRunning(true),
      mIsJumping(false),
      mPlayedJumpSound(false),
      mGravity(3750.f)
{
    mJumpSound.setVolume(100);
    // top, left, width, height
    mAnimation.setTexture(context.mTextures->get(Textures::BlueSkirt));
    mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
    mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 3;
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height) + 6.f);

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
    mSprite.setPosition(x, y);
}

void Player::changeTexture(sf::Texture &texture1, sf::Texture &texture2)
{
    mAnimation.setTexture(texture1);
    mSprite.setTexture(texture2);
}

void Player::handleEvent(User user)
{
    if (user.isSpacePressed)
    {
        if (!mIsJumping)
        {
            mVelocity.y = -1500.f;
            mPlayedJumpSound = false;
            mIsJumping = true;
        }
    }
    else if (!mIsJumping)
    {
        mVelocity.y = 0.f;
    }
}

void Player::update(sf::Time dt, float groundHeight)
{
    if (groundHeight == 0.f)
    {
        mAnimation.setFrame(8);
        return;
    }
    // jumping mechanic
    if (mPosition.y < groundHeight) // above ground
    {
        mVelocity.y += mGravity * dt.asSeconds(); // add gravity
        mIsJumping = true;
    }
    else if (mPosition.y > groundHeight) // below ground
    {
        mPosition.y = groundHeight;
    }

    if (mPosition.y == groundHeight)
        mIsJumping = false;

    mAnimation.setPosition(mPosition);
    mSprite.setPosition(mPosition);
    mPosition.y += mVelocity.y * dt.asSeconds();
    mPosition.x += mVelocity.x * dt.asSeconds();

    if (mIsJumping && !mPlayedJumpSound)
    {
        mJumpSound.play();
        mPlayedJumpSound = true;
    }
    if (mIsJumping)
        mAnimation.setFrame(0);
    else 
        mAnimation.update(dt);
}

bool Player::isJumping() const
{
    return mIsJumping;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mAnimation, states);
}

