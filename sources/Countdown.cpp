#include <MINE/Countdown.hpp>

Countdown::Countdown(StateStack &stack, Context context)
    : State(stack, context),
      mText(" ", context.mFonts->get(Fonts::Caro), 100),
      mShowText(false),
      mTime(0.f),
      mTitle("Continue to play in...", context.mFonts->get(Fonts::Main), 70)
    //   mFirstSound(context.mSoundBuffers->get(Sounds::First))
{
    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
    // mFirstSound.setVolume(100);
    // mFirstSound.play();
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color::White);
    mTitle.setPosition(0.5f * 1600.f, 0.4f * 900.f);

    mText.setColor(sf::Color::White);
    mText.setPosition(0.5f * 1600.f, 0.5f * 900.f );
}

bool Countdown::handleEvent(User user)
{   
    return false;
}

bool Countdown::update(sf::Time dt)
{
    mTime += dt.asSeconds();
    if (mTime < 1.5f)
        mText.setString(" ");
    else if (mTime < 2.2f)
        mText.setString("3");
    else if (mTime < 2.5f)
        mText.setString(" ");
    else if (mTime < 3.2f)
        mText.setString("2");
    else if (mTime < 3.5f)
        mText.setString(" ");
    else if (mTime < 4.5f)
        mText.setString("1");
    else
        requestStackPop();

    return false;
}

void Countdown::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundRect, states);
    target.draw(mTitle, states);

    target.draw(mText, states);
}
