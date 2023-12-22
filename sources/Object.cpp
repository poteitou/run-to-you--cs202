#include <MINE/Object.hpp>

Object::Object(State::Context context, std::string type, float x, float y)
    : mAnimation(),
      mCollideSound(),
      mIsDone(false),
      mIsCollide(false), 
      mAlreadyCollide(false)
{
    mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Collide));
    if (type == "Milktea")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Cat")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Cat));
        mWidth = mAnimation.getTexture()->getSize().x / 6;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(6);
    }
    else if (type == "Friend")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Rock")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Bird")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Bird));
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
    if (mType == "Heart" && mAlreadyCollide)
    {
        if (mAnimation.getPosition().x > 1250.f)
        {
            mIsDone = true;
            mAnimation.setPosition(-mWidth * 2.f, mAnimation.getPosition().y);
        }
        else
            mAnimation.move(1600.f * dt.asSeconds(), -900.f * dt.asSeconds());
    }
    else
    {
        mAnimation.move(-scrollSpeed * dt.asSeconds(), 0.f);
    }
    if (!mAlreadyCollide && Collision::pixelPerfectTest(player.getSprite(), mAnimation.getSprite(), (sf::Uint8)0U))
    {
        mCollideSound.play();
        mAlreadyCollide = true;
        mIsCollide = true;
    }
    else
    {
        mIsCollide = false;
    }

    mAnimation.update(dt);
}

bool Object::isCollide()
{
    return mIsCollide;
}

bool Object::isDone()
{
    return mIsDone;
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mAnimation, states);
}
