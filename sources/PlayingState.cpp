#include <MINE/PlayingState.hpp>

PlayingState::PlayingState(StateStack &stack, Context context)
    : State(stack, context), 
      mBackgroundSprite(),
      mGroundSprite(),
      mDistanceText("", context.mFonts->get(Fonts::Main), 50),
      mPlayer(context),
      mGroundHeight(900.f - 200.f),
      mScrollSpeed(200.f), 
      mDistance(0.f),
      mIsPaused(false)
{
    sf::Texture &backgroundTexture = context.mTextures->get(Textures::PinkBackground);
    sf::Texture &groundTexture = context.mTextures->get(Textures::Ground);

    sf::IntRect textureRect(0.f, 0.f, 1600.f, 900.f);
    
    // prepare for background and ground continuous scrolling
    for (int i = 0; i < 2; i++)
    {
        mBackgroundSprite[i].setTextureRect(textureRect);
        mBackgroundSprite[i].setTexture(backgroundTexture);
        mGroundSprite[i].setTextureRect(textureRect);
        mGroundSprite[i].setTexture(groundTexture);
    }
    mBackgroundSprite[0].setPosition(0.f, 0.f);
    mBackgroundSprite[1].setPosition(1600.f, 0.f);
    mGroundSprite[0].setPosition(0.f, 0.f);
    mGroundSprite[1].setPosition(1600.f, 0.f);

    mPlayer.setPosition(150.f, mGroundHeight);
    mPlayer.setVelocity(0.f, 0.f);

    // prepare for distance text
    mDistanceText.setPosition(50.f, 100.f);
    mDistanceText.setColor(sf::Color::Black);
}

bool PlayingState::handleEvent(User user)
{
    if (user.isEscapePressed && !mIsPaused)
    {
        mIsPaused = true;
    }
    if (user.isEnterPressed && mIsPaused)
    {
        mIsPaused = false;
    }
    if (!mIsPaused)
        mPlayer.handleEvent(user);
    return true;
}

bool PlayingState::update(sf::Time dt)
{
    if (!mIsPaused)
    {
        mDistance += mScrollSpeed * dt.asSeconds();
        mDistanceText.setString(std::to_string((int)mDistance) + " m");
        int distance = 0;
        for (int i = 0; i < 2; i++)
        {
            mBackgroundSprite[i].move(-mScrollSpeed / 3 * dt.asSeconds(), 0.f);
            mGroundSprite[i].move(-mScrollSpeed * dt.asSeconds(), 0.f);
            if (mGroundSprite[i].getPosition().x < -1600.f)
                mGroundSprite[i].setPosition(1600.f, 0.f);
            if (mBackgroundSprite[i].getPosition().x < -1600.f)
                mBackgroundSprite[i].setPosition(1600.f, 0.f);
        }
        mPlayer.update(dt, mGroundHeight);
    }
    return true;
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    /*
    // this code will cause unexpected behavior
    for (int i = 0; i < 2; i++)
    {
        mWindow.draw(mBackgroundSprite[i]);
        mWindow.draw(mGroundSprite[i]);
    }
    */

    for (int i = 0; i < 2; i++)
    {
        target.draw(mBackgroundSprite[i], states);
    }
    for (int i = 0; i < 2; i++)
    {
        target.draw(mGroundSprite[i], states);
    }
    target.draw(mDistanceText, states);
    target.draw(mPlayer, states);
}   