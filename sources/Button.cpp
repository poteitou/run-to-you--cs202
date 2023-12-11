#include <MINE/Button.hpp>

Button::Button(State::Context context)
    : mAnimation(context.mTextures->get(Textures::Button)),
      mPressedSound(context.mSoundBuffers->get(Sounds::Button)),
      mSprite(context.mTextures->get(Textures::Button)),
      mText("", context.mFonts->get(Fonts::Main), 70),
      mIsPressed(false), mIsHovered(false), mPlayedPressedSound(false)
{
    mPressedSound.setVolume(100);
    // top, left, width, height
    mWidth = context.mTextures->get(Textures::Button).getSize().x;
    mHeight = context.mTextures->get(Textures::Button).getSize().y / 3;
    mSprite.setTextureRect(sf::IntRect(0, mHeight * 2, mWidth, mHeight));
    centerOrigin(mSprite);
    mText.setColor(sf::Color::Black);
    mAnimation.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mAnimation.setNumFrames(2);
    mAnimation.setDuration(sf::seconds(0.6f));
    mAnimation.centerOrigin();
}

void Button::centerOrigin(sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void Button::centerOrigin(sf::Text &text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

sf::FloatRect Button::getLocalBounds()
{
    return mSprite.getLocalBounds();
}

void Button::setText(const std::string &text, unsigned int size = 70)
{
    mText.setString(text);
    mText.setCharacterSize(size);
    centerOrigin(mText);
}

void Button::setPosition(float x, float y)
{
    mAnimation.setPosition(x, y);
    mSprite.setPosition(x, y);
    mText.setPosition(x, y - 16.f);
}

void Button::handleEvent(User user)
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    if (bounds.contains(static_cast<sf::Vector2f>(user.mousePosition)))
    {
        mIsHovered = true;
        if (user.isMousePressed)
            mIsPressed = true;
        mText.setColor(sf::Color::White);
    }
    else
    {
        mIsHovered = false;
        mText.setColor(sf::Color::Black);
    }
}

void Button::update(sf::Time dt)
{
    mText.setPosition(mAnimation.getPosition().x, mAnimation.getPosition().y - 16.f);
    if (mIsPressed)
    {
        mAnimation.update(dt);
        if (!mPlayedPressedSound)
        {
            mPressedSound.play();
            mPlayedPressedSound = true;
        }
        if (mAnimation.isNotLastFrame())
        {    mText.setPosition(mAnimation.getPosition().x, mAnimation.getPosition().y + 16.f);
        }
    }
    else if (mIsHovered)
    {
        changeTexture(Selected);
    }
    else
    {
        changeTexture(Normal);
    }
}

bool Button::isPressed() const
{
    return mAnimation.isFinished();
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    if (mIsPressed)
    {
        target.draw(mAnimation, states);
        target.draw(mText, states);
    }
    else
    {
        target.draw(mSprite, states);
        target.draw(mText, states);
    }
}

void Button::changeTexture(Type type)
{
    sf::IntRect textureRect(0, mHeight * type, mWidth, mHeight);
    mSprite.setTextureRect(textureRect);
}
