#include <MINE/Button.hpp>

Button::Button(State::Context context)
    : mSprite(context.mTextures->get(Textures::Button))
    // , mSound(context.mSounds->get(Sounds::Button))
    , mText("", context.mFonts->get(Fonts::Main), 70)
    , mIsPressed(false), mIsHovered(false)
{
    // top, left, width, height
    mSprite.setTextureRect(sf::IntRect(0, 0, 400, 100));
    centerOrigin(mSprite);
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
    mSprite.setPosition(x, y);
    mText.setPosition(x, y);
}

void Button::setTexture(Type type, const sf::Texture &texture)
{
    mSprite.setTexture(texture);
}

void Button::setTexture(Type type, const sf::Texture &texture, const sf::IntRect &textureRect)
{
    mSprite.setTexture(texture);
    mSprite.setTextureRect(textureRect);
}

void Button::handleEvent(User user)
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    if (bounds.contains(static_cast<sf::Vector2f>(user.mousePosition)))
    {
        mIsHovered = true;
        changeTexture(Selected);
        if (user.isMousePressed)
        {
            mIsPressed = true;
            changeTexture(Pressed);
        }
    }
    else
    {
        mIsPressed = false;
        mIsHovered = false;
        changeTexture(Normal);
    }
}

bool Button::isPressed() const
{
    return mIsPressed;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}

void Button::changeTexture(Type type)
{
    sf::IntRect textureRect(0, 100 * type, 400, 100);
    mSprite.setTextureRect(textureRect);
}
