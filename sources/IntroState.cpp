#include <MINE/IntroState.hpp>

IntroState::IntroState(StateStack &stack, Context context)
    : State(stack, context)
    , mText()
    , mShowText(true)
    , mTextEffectTime(sf::Time::Zero)
{
    mBackgroundSprite.setTexture(context.mTextures->get(Textures::PinkBackground));

    mText.setFont(context.mFonts->get(Fonts::Main));
    mText.setString("Press enter to start");
    mText.setColor(sf::Color::White);
    mText.setCharacterSize(50);
    mText.setPosition(0.5f * 1600.f - 0.5f * mText.getLocalBounds().width, 0.75f * 900.f - 0.5f * mText.getLocalBounds().height);
}

bool IntroState::handleEvent(User user)
{
    if (user.isEnterPressed)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }

    return true;
}

bool IntroState::update(sf::Time dt)
{
    mTextEffectTime += dt;

    if (mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }

    return true;
}

void IntroState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mBackgroundSprite, states);

    if (mShowText)
        target.draw(mText, states);
}
