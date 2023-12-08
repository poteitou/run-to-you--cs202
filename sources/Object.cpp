#include <MINE/Object.hpp>

Object::Object(State::Context context, Type type, float x, float y)
    : mPlayedCollideSound(false),
      mAnimation(),
      mCollideSound(),
    //   mAnimation(context.mTextures->get(Textures::Heart)),
    //   mCollideSound(context.mSoundBuffers->get(Sounds::Jump)),
      mIsCollide(false)
{
    // switch (type)
    // {
    // case Object::Type::Heart:
        sf::Texture &tmpTexture = context.mTextures->get(Textures::Heart);
        mAnimation.setTexture(tmpTexture);
        // sf::SoundBuffer& tmpSoundBuffer = context.mSoundBuffers->get(Sounds::Jump);
        // mCollideSound.setBuffer(tmpSoundBuffer);
        // mCollideSound.setBuffer(context.mSoundBuffers->get(Sounds::Jump));
    //     break;
    // }
    // mCollideSound.setVolume(100);
    // top, left, width, height
    mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 1;
    mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 2;

    mAnimation.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mAnimation.setNumFrames(2);
    mAnimation.setDuration(sf::seconds(1.f));
    mAnimation.centerOrigin();
    mAnimation.setRepeating(true);

    mType = type;
    setPosition(x, y);
}

void Object::centerOrigin(sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

sf::FloatRect Object::getGlobalBounds()
{
    return mAnimation.getGlobalBounds();
}

Object::Type Object::getType()
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

void Object::update(sf::Time dt, float scrollSpeed, float groundHeight)
{
    mAnimation.move(-scrollSpeed * dt.asSeconds(), 0.f);

    if (!mPlayedCollideSound)
    {
        mCollideSound.play();
        mPlayedCollideSound = true;
    }
    if (mIsCollide)
        mAnimation.setFrame(0);
    else 
        mAnimation.update(dt);
}

bool Object::isCollide(const Player &player) 
{
    return Collision::pixelPerfectTest(player.getSprite(), mAnimation.getSprite(), (sf::Uint8)0U);
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mAnimation, states);
}

