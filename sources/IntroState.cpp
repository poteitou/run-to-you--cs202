#include <MINE/IntroState.hpp>

IntroState::IntroState(StateStack &stack, Context context)
    : State(stack, context),
      mText("Press enter to start", context.mFonts->get(Fonts::Main), 50),
      mShowText(false),
      mTextEffectTime(sf::Time::Zero),
      mTitle("RUN", context.mFonts->get(Fonts::Caro), 200),
      mTitleSprite(context.mTextures->get(Textures::ToYou)),
      mGirl(context.mTextures->get(Textures::BlueSkirt)),
      mFirstSound(context.mSoundBuffers->get(Sounds::First))
{
    mFirstSound.setVolume(100);
    mFirstSound.play();
    mTitle.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color(63, 105, 195));
    // outside the screen to the left
    mTitle.setPosition(-180.f, 0.25f * 900.f);
    // mTitle.setPosition(0.5f * 1600.f - 180.f, 0.25f * 900.f);

    int mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
    int mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 3;
    mGirl.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mGirl.setNumFrames(8);
    mGirl.setDuration(sf::seconds(0.8f));
    mGirl.centerBottom();
    mGirl.setRepeating(true);
    // outside the screen to the left
    mGirl.setPosition(-180.f - 1.75f * mWidth, 0.25f * 900.f + 0.5f * mHeight);
    // mGirl is on the left of mTitle
    // mGirl.setPosition(0.5f * 1600.f - 180.f - 1.75f * mWidth, 0.25f * 900.f + 0.5f * mHeight);

    // outside the screen to the right
    mTitleSprite.setPosition(1600.f + 180.f, 0.25f * 900.f);
    // mTitleSprite.setPosition(0.5f * 1600.f, 0.25f * 900.f);

    mBackgroundSprite.setTexture(context.mTextures->get(Textures::PinkBackground));
    mBackgroundSprite.setPosition(0.f, 0.f);

    mText.setColor(sf::Color::White);
    mText.setPosition(0.5f * 1600.f - 0.5f * mText.getLocalBounds().width, 0.75f * 900.f - 0.5f * mText.getLocalBounds().height);
}

bool IntroState::handleEvent(User user)
{
    if (mTitle.getPosition().x < 0.5f * 1600.f - 180.f)
        return true;
    if (mTitleSprite.getPosition().x > 0.5f * 1600.f)
        return true;
    requestStackPop();
    requestStackPush(States::Menu);
    return true;
}

bool IntroState::update(sf::Time dt)
{
    mGirl.update(dt);
    if (mTitle.getPosition().x > 0.5f * 1600.f - 180.f)
    {
        mTitle.setPosition(0.5f * 1600.f - 180.f, 0.25f * 900.f);
        mGirl.setPosition(0.5f * 1600.f - 180.f - 1.75f * mGirl.getFrameSize().x, 0.25f * 900.f + 0.5f * mGirl.getFrameSize().y);
    }
    if (mTitleSprite.getPosition().x < 0.5f * 1600.f)
        mTitleSprite.setPosition(0.5f * 1600.f, 0.25f * 900.f);
        
    if (mTitle.getPosition().x < 0.5f * 1600.f - 180.f)
    {
        mTitle.move(1000.f * dt.asSeconds(), 0.f);
        mGirl.move(1000.f * dt.asSeconds(), 0.f);
    }
    else if (mTitleSprite.getPosition().x > 0.5f * 1600.f)
        mTitleSprite.move(-1000.f * dt.asSeconds(), 0.f);
    // else
    // {
    //     mTextEffectTime += dt;

    //     if (mTextEffectTime >= sf::seconds(0.5f))
    //     {
    //         mShowText = !mShowText;
    //         mTextEffectTime = sf::Time::Zero;
    //     }
    // }

    return true;
}

void IntroState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mBackgroundSprite, states);
    target.draw(mTitle, states);
    target.draw(mTitleSprite, states);
    target.draw(mGirl, states);

    // if (mShowText)
    //     target.draw(mText, states);
}
