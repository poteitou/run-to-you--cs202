#include <MINE/PlayingState.hpp>

PlayingState::PlayingState(StateStack &stack, Context context)
    : State(stack, context), 
      mBackgroundSprite(), 
      mGroundSprite(), 
      mView(context.mWindow->getDefaultView()), 
      mScrollSpeed(100.f), 
      mViewBound(0.f, 0.f, 10000.f, mView.getSize().y),
      mPlayer(context)
{
    sf::Texture &backgroundTexture = context.mTextures->get(Textures::PinkBackground);
    sf::Texture &groundTexture = context.mTextures->get(Textures::Ground);
    backgroundTexture.setRepeated(true);
    groundTexture.setRepeated(true);

    sf::IntRect textureRect(0.f, 0.f, 10000.f, mView.getSize().y);

    mBackgroundSprite.setTextureRect(textureRect);
    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setPosition(0.f, 0.f);

    mGroundSprite.setTextureRect(textureRect);
    mGroundSprite.setTexture(groundTexture);
    mGroundSprite.setPosition(0.f, 0.f);
    jump = false;

    mGroundHeight = 900.f - 200.f;
    mPlayer.setPosition(100.f, mGroundHeight);
    mPlayer.setVelocity(100.f, 0.f);
}

bool PlayingState::handleEvent(User user)
{
    if (user.isSpacePressed)
    {
        jump = true;
    }
    if (user.isEnterPressed)
    {
        jump = false;
    }
    mPlayer.handleEvent(user);
    return true;
}

bool PlayingState::update(sf::Time dt)
{
    if (!jump)
    {
        mView.move(mScrollSpeed * dt.asSeconds(), 0.f);	
        mGroundSprite.move(-100.f * dt.asSeconds(), 0.f);
    }
    mPlayer.update(dt, mGroundHeight);
    return true;
}

void PlayingState::render()
{
    sf::RenderWindow &mWindow = *getContext().mWindow;
    mWindow.setView(mView);
    mWindow.draw(mBackgroundSprite);
    mWindow.draw(mGroundSprite);
    mWindow.draw(mPlayer);
}