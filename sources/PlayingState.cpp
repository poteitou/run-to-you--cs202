#include <MINE/PlayingState.hpp>

PlayingState::PlayingState(StateStack &stack, Context context)
    : State(stack, context),
      mBackgroundSprite(),
      mGroundSprite(),
      mLives(context.mTextures->get(Textures::Life)),
      mDistanceText("", context.mFonts->get(Fonts::Main), 50),
      mPlayer(context),
      mGroundHeight(900.f - 80.f),
      mScrollSpeed(400.f),
      mDistance(0.f),
      mCntLives(3)
{
    sf::Texture &backgroundTexture = context.mTextures->get(Textures::PinkBackground);
    sf::Texture &groundTexture = context.mTextures->get(Textures::Ground);

    sf::IntRect textureRect(0.f, 0.f, 1600.f, 900.f);

    // prepare for background and ground continuous scrolling
    for (int i = 0; i < 3; i++)
    {
        mBackgroundSprite[i].setTextureRect(textureRect);
        mBackgroundSprite[i].setTexture(backgroundTexture);
        mGroundSprite[i].setTextureRect(textureRect);
        mGroundSprite[i].setTexture(groundTexture);
    }
    mBackgroundSprite[0].setPosition(0.f, 0.f);
    mBackgroundSprite[1].setPosition(1600.f, 0.f);
    mBackgroundSprite[2].setPosition(3200.f, 0.f);
    mGroundSprite[0].setPosition(0.f, 0.f);
    mGroundSprite[1].setPosition(1600.f, 0.f);
    mGroundSprite[2].setPosition(3200.f, 0.f);

    mLives.setTextureRect(sf::IntRect(0, mCntLives * 80, 256, 80));
    mLives.setPosition(1600.f - 256.f - 50.f, 50.f);

    mPaused.setBuffer(context.mSoundBuffers->get(Sounds::Paused));
    mPaused.setVolume(100);
    mGameOver.setBuffer(context.mSoundBuffers->get(Sounds::GameOver));
    mGameOver.setVolume(100);

    mPlayer.setPosition(150.f, 300.f);
    mPlayer.setVelocity(0.f, 0.f);

    // prepare for distance text
    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);

    // initialize obstacle queue, include some obstacles start from position 2000.f
    mTypeObject = {"Milktea", "Cat", "Friend", "Rock", "Bird", "Heart"};
    srand(time(NULL));
    int obstacleType = rand() % (mTypeObject.size() - 1);

    mObstacleQueue.push_back(Object(context, mTypeObject[obstacleType], 2400.f, mGroundHeight));
}

void PlayingState::createObstacle()
{
    srand(time(NULL));
    if (mObstacleQueue.back().getPosition().x < 1600.f)
    {
        bool hasHeart = false;
        bool hasCoin = false;
        
        while (mObstacleQueue.back().getPosition().x < 3200.f)
        {
            int obstacleType = rand() % mTypeObject.size();
            if (mTypeObject[obstacleType] == "Heart")
            {
                if (hasHeart)
                    obstacleType = rand() % (mTypeObject.size() - 1);
                hasHeart = true;
            }

            int randPos = rand() % 100;
            float minDis = mScrollSpeed * 2 / 3;
            float maxDis = mScrollSpeed * 2;
            float delta = minDis + (maxDis - minDis) * randPos / 100.f;
            mObstacleQueue.push_back(Object(getContext(), mTypeObject[obstacleType], mObstacleQueue.back().getPosition().x + delta, mGroundHeight));
        }
    }
}

bool PlayingState::handleEvent(User user)
{
    if (user.isEscapePressed)
    {
        mPaused.play();
        requestStackPush(States::Paused);
    }
    mPlayer.handleEvent(user);
    return true;
}

bool PlayingState::update(sf::Time dt)
{
    for (auto &obstacle : mObstacleQueue)
    {
        if (obstacle.isDone() && mCntLives < 3) 
        {
            ++mCntLives;
            mLives.setTextureRect(sf::IntRect(0, mCntLives * 80, 256, 80));
        }
        else if (obstacle.isCollide() && obstacle.getType() != "Heart") 
        {
            --mCntLives;
            mLives.setTextureRect(sf::IntRect(0, mCntLives * 80, 256, 80));
            if (mCntLives == 0)
            {
                mGameOver.play();
                requestStackPush(States::GameOver);
            }
        }
    }

    mDistance += mScrollSpeed * dt.asSeconds();
    mDistanceText.setString("Distance: " + std::to_string((int)mDistance / 100) + "m");
    if (!mObstacleQueue.empty() && mObstacleQueue.front().isOutOfScreen())
        mObstacleQueue.pop_front();
    for (auto &obstacle : mObstacleQueue)
    {
        obstacle.update(dt, mScrollSpeed, mPlayer);
    }
    createObstacle();

    mPlayer.update(dt, mGroundHeight);
    for (int i = 0; i < 3; i++)
    {
        mBackgroundSprite[i].move(-mScrollSpeed / 4 * dt.asSeconds(), 0.f);
        mGroundSprite[i].move(-mScrollSpeed * dt.asSeconds(), 0.f);
    }
    for (int i = 0; i < 3; i++)
    {
        if (mBackgroundSprite[i].getPosition().x < -1600.f)
            mBackgroundSprite[i].setPosition(mBackgroundSprite[(i + 2) % 3].getPosition().x + 1600.f, 0.f);
        if (mGroundSprite[i].getPosition().x < -1600.f)
            mGroundSprite[i].setPosition(mGroundSprite[(i + 2) % 3].getPosition().x + 1600.f, 0.f);
    }
    if (mScrollSpeed < 2000.f)
        mScrollSpeed += 0.2f;
    return true;
}

void PlayingState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &sprite : mBackgroundSprite)
    {
        target.draw(sprite, states);
    }
    for (auto &sprite : mGroundSprite)
    {
        target.draw(sprite, states);
    }
    target.draw(mLives, states);
    target.draw(mDistanceText, states);

    for (auto &obstacle : mObstacleQueue)
    {
        target.draw(obstacle, states);
    }
    target.draw(mPlayer, states);
}