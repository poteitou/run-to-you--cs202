#include <MINE/Object.hpp>

Object::Object(State::Context context, std::string type, float x, float y)
    : mAnimation(),
      mCollideSound(),
      mIsDone(false),
      mIsCollide(false), 
      mAlreadyCollide(false)
{
    mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Collide));
    if (type == "Tree")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Tree));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(1);
    }
    else if (type == "Rock")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Rock));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(1);
    }
    else if (type == "RockHeap")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::RockHeap));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(1);
    }
    else if (type == "Bird1")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::BirdGreen));
        mWidth = mAnimation.getTexture()->getSize().x / 8;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(8);
        y -= 65.f;
    }
    else if (type == "Bird2")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Bird));
        mWidth = mAnimation.getTexture()->getSize().x / 8;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(8);
        y -= 200.f;
    }
    else if (type == "Bird3")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Bird));
        mWidth = mAnimation.getTexture()->getSize().x / 8;
        mHeight = mAnimation.getTexture()->getSize().y;
        mAnimation.setNumFrames(8);
        y -= 360.f;
    }
    else if (type == "Heart1")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
    }
    else if (type == "Heart2")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
        y -= 120.f;
    }
    else if (type == "Heart3")
    {
        mAnimation.setTexture(context.mTextures->get(Textures::Heart));
        mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Heart));
        mWidth = mAnimation.getTexture()->getSize().x;
        mHeight = mAnimation.getTexture()->getSize().y / 4;
        mAnimation.setNumFrames(4);
        y -= 200.f;
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
    if ((mType == "Heart1" || mType == "Heart2" || mType == "Heart3") && mAlreadyCollide)
    {
        if (mAnimation.getPosition().x > 1250.f)
        {
            mIsDone = true;
            mAnimation.setPosition(-mWidth * 2.f, mAnimation.getPosition().y);
        }
        else
            mAnimation.move(2 * 1600.f * dt.asSeconds(), 2 * -900.f * dt.asSeconds());
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
