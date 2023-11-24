#include <MINE/PlayingState.hpp>

PlayingState::PlayingState(StateStack &stack, Context context)
    : State(stack, context),  
      mView(context.mWindow->getDefaultView()), 
      mScrollSpeed(200.f), 
      mViewBound(0.f, 0.f, 10000.f, mView.getSize().y),
      mPlayer(context)
{
    sf::Texture &backgroundTexture = context.mTextures->get(Textures::PinkBackground);
    sf::Texture &groundTexture = context.mTextures->get(Textures::Ground);
    backgroundTexture.setRepeated(true);
    groundTexture.setRepeated(true);

    sf::IntRect textureRect(0.f, 0.f, 1600.f, 900.f);

    mBackgroundSprite[0].setTextureRect(textureRect);
    mBackgroundSprite[0].setTexture(backgroundTexture);
    mBackgroundSprite[0].setPosition(0.f, 0.f);
    mBackgroundSprite[1].setTextureRect(textureRect);
    mBackgroundSprite[1].setTexture(backgroundTexture);
    mBackgroundSprite[1].setPosition(1600.f, 0.f);

    mGroundSprite[0].setTextureRect(textureRect);
    mGroundSprite[0].setTexture(groundTexture);
    mGroundSprite[0].setPosition(0.f, 0.f);
    mGroundSprite[1].setTextureRect(textureRect);
    mGroundSprite[1].setTexture(groundTexture);
    mGroundSprite[1].setPosition(1600.f, 0.f);
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
    // if (!jump)
    // {
    for (int i = 0; i < 2; i++)
    {
        mBackgroundSprite[i].move(-mScrollSpeed / 3 * dt.asSeconds(), 0.f);
        mGroundSprite[i].move(-mScrollSpeed * dt.asSeconds(), 0.f);
        if (mGroundSprite[i].getPosition().x < -1600.f)
        {
            mGroundSprite[i].setPosition(1600.f, 0.f);
        }
        
    }
        // mView.move(mScrollSpeed * dt.asSeconds(), 0.f);	
        // mGroundSprite.move(-100.f * dt.asSeconds(), 0.f);
    // }
    mPlayer.update(dt, mGroundHeight);
    return true;
}

void PlayingState::render()
{
    sf::RenderWindow &mWindow = *getContext().mWindow;
    mWindow.setView(mView);
    for (int i = 0; i < 2; i++)
    {
        mWindow.draw(mBackgroundSprite[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        mWindow.draw(mGroundSprite[i]);
    }
    mWindow.draw(mPlayer);
}