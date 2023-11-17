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
    mText.setPosition(0.5f * context.mWindow->getSize().x - 0.5f * mText.getLocalBounds().width, 0.75f * context.mWindow->getSize().y - 0.5f * mText.getLocalBounds().height);
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

void IntroState::render()
{
    sf::RenderWindow &mWindow = *getContext().mWindow;
    mWindow.draw(mBackgroundSprite);

    if (mShowText)
        mWindow.draw(mText);
}
