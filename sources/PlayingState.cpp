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
    for (int i = 0; i < 3; i++)
    {
        mBackgroundSprite[i].setTextureRect(textureRect);
        mBackgroundSprite[i].setTexture(backgroundTexture);
        mGroundSprite[i].setTextureRect(textureRect);
        mGroundSprite[i].setTexture(groundTexture);
    }
    mBackgroundSprite[0].setPosition(0.f, 0.f);
    mBackgroundSprite[1].setPosition(1599.f, 0.f);
    mBackgroundSprite[2].setPosition(3198.f, 0.f);
    mGroundSprite[0].setPosition(0.f, 0.f);
    mGroundSprite[1].setPosition(1599.f, 0.f);
    mGroundSprite[2].setPosition(3199.f, 0.f);

    mPlayer.setPosition(150.f, mGroundHeight);
    mPlayer.setVelocity(0.f, 0.f);

    // prepare for distance text
    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);

    // initialize obstacle queue, include some obstacles start from position 1800.f
    mTypeObject = {"Milktea", "Cat", "Friend", "Rock", "Bird", "Heart"};
    srand(time(NULL));
    int obstacleType = rand() % (mTypeObject.size() - 1);

    mObstacleQueue.push_back(Object(context, mTypeObject[obstacleType], 1800.f, mGroundHeight));
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
            if (mTypeObject[obstacleType] == "Heart" && hasHeart)
                obstacleType = rand() % (mTypeObject.size() - 1);

            int randPos = rand() % 4;
            float delta;
            switch (randPos)
            {
            case 0:
                delta = mScrollSpeed;
                break;
            case 1:
                delta = mScrollSpeed * 1.5f;
                break;
            case 2:
                delta = mScrollSpeed;
                break;
            case 3:
                delta = mScrollSpeed * 2.f;
                break;
            }

            mObstacleQueue.push_back(Object(getContext(), mTypeObject[obstacleType], mObstacleQueue.back().getPosition().x + delta, mGroundHeight));
        }
    }
}

bool PlayingState::handleEvent(User user)
{
    if (user.isEscapePressed)
    {
        // mIsPaused = true;
        requestStackPush(States::Paused);
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
        mDistanceText.setString("Distance: " + std::to_string((int)mDistance / 100) + "m");
        if (!mObstacleQueue.empty() && mObstacleQueue.front().isOutOfScreen())
            mObstacleQueue.pop_front();
        for (auto &obstacle : mObstacleQueue)
        {
            obstacle.update(dt, mScrollSpeed, mPlayer);
        }
        createObstacle();

        for (auto &obstacle : mObstacleQueue)
        {
            if (obstacle.isCollide())
            {
                mIsPaused = true;
                break;
            }
        }
        mPlayer.update(dt, mGroundHeight);
        for (int i = 0; i < 3; i++)
        {
            mBackgroundSprite[i].move(-mScrollSpeed / 4 * dt.asSeconds(), 0.f);
            mGroundSprite[i].move(-mScrollSpeed * dt.asSeconds(), 0.f);
        }
        for (int i = 0; i < 3; i++)
        {
            if (mBackgroundSprite[i].getPosition().x < -1599.f)
                mBackgroundSprite[i].setPosition(mBackgroundSprite[(i + 2) % 3].getPosition().x + 1599.f, 0.f);
            if (mGroundSprite[i].getPosition().x < -1599.f)
                mGroundSprite[i].setPosition(mGroundSprite[(i + 2) % 3].getPosition().x + 1599.f, 0.f);
        }
        if ((int)(mDistance) / 100 % 100 == 0)
            mScrollSpeed += 20.f;
    }
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
    target.draw(mDistanceText, states);

    for (auto &obstacle : mObstacleQueue)
    {
        target.draw(obstacle, states);
    }
    target.draw(mPlayer, states);
}