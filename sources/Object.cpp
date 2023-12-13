#include <MINE/Object.hpp>

Object::Object(State::Context context, std::string type, float x, float y)
    : mPlayedCollideSound(false),
      mAnimation(),
      mCollideSound(),
      mIsCollide(false), 
      mAlreadyCollide(false)
{
    if (type == "Milktea")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Jump));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Cat")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Cat));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Jump));
        mWidth = mAnimation.getTexture()->getSize().x / 6;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(6);
    }
    else if (type == "Friend")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Jump));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Rock")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Jump));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Bird")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Bird));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Jump));
        mWidth = mAnimation.getTexture()->getSize().x / 8;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(8);
        y -= 300.f;
    }
    else if (type == "Heart")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
        y -= 100.f;
    }
    mCollideSound.setVolume(100);

    mAnimation.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mAnimation.setDuration(sf::seconds(0.7f));
    mAnimation.centerBottom();
    mAnimation.setRepeating(true);

    mType = type;
    setPosition(x, y);
}

void Object::centerBottom(sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height));
}

sf::FloatRect Object::getGlobalBounds()
{
    return mAnimation.getGlobalBounds();
}

std::string Object::getType()
{
    return mType;
}

void Object::setPosition(float x, float y)
{
    mAnimation.setPosition(x, y);
}

sf::Vector2f Object::getPosition()
{
    return mAnimation.getPosition();
}

bool Object::isOutOfScreen()
{
    return mAnimation.getPosition().x < -mWidth;
}

void Object::changeTexture(sf::Texture &texture)
{
    mAnimation.setTexture(texture);
}

void Object::handleEvent(User user)
{
}

void Object::update(sf::Time dt, float scrollSpeed, const Player &player)
{
    mAnimation.move(-scrollSpeed * dt.asSeconds(), 0.f);
    if (!mAlreadyCollide && Collision::pixelPerfectTest(player.getSprite(), mAnimation.getSprite(), (sf::Uint8)0U))
    {
        if (!mIsCollide)
        {
            mAlreadyCollide = true;
            mIsCollide = true;
            mPlayedCollideSound = false;
        }
    }
    else
    {
        mIsCollide = false;
    }

    if (mIsCollide && !mPlayedCollideSound)
    {
        mCollideSound.play();
        mPlayedCollideSound = true;
    }
    if (mIsCollide)
        mAnimation.setFrame(0);
    else
        mAnimation.update(dt);
}

bool Object::isCollide()
{
    return mIsCollide;
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mAnimation, states);
}
