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
    mDistanceText.setPosition(50.f, 100.f);
    mDistanceText.setColor(sf::Color::Black);

    // initialize obstacle queue, include some obstacles start from position 1800.f
    Animation tmpAnimation;
    srand(time(NULL));
    int obstacleType = rand() % (TypeCount - 2);
            
    tmpAnimation.setPosition(1800.f, mGroundHeight);
    // switch (obstacleType)
    // {
    // case 0:
    //     tmpTexture = context.mTextures->get(Textures::Milktea);
    //     break;
    // case 1:
    //     tmpTexture = context.mTextures->get(Textures::Cat);
    //     break;
    // case 2:
    //     tmpTexture = context.mTextures->get(Textures::Friend);
    //     break;
    // case 3:
    //     tmpTexture = context.mTextures->get(Textures::Rock);
    //     break;
    // default:
    //     break;
    // }
    sf::Texture &tmpTexture = context.mTextures->get(Textures::Heart);
    tmpAnimation.setTexture(tmpTexture);
    int width = tmpTexture.getSize().x / 1;
    int height = tmpTexture.getSize().y / 2;

    tmpAnimation.setFrameSize(sf::Vector2i(width, height));
    tmpAnimation.setNumFrames(2);
    tmpAnimation.setDuration(sf::seconds(1.f));
    tmpAnimation.centerOrigin();
    tmpAnimation.setRepeating(true);
    mObstacleQueue.push_back(std::make_pair(obstacleType, tmpAnimation));
}

void PlayingState::createObstacle()
{
    srand(time(NULL));
    if (mObstacleQueue.back().second.getPosition().x < 1600.f)
    {
        bool hasHeart = false;
        bool hasCoin = false;
        
        while (mObstacleQueue.back().second.getPosition().x < 3200.f)
        {
            Animation tmpAnimation;
            int obstacleType = rand() % TypeCount;
            if (obstacleType == Heart && hasHeart)
                obstacleType = rand() % (TypeCount - 1);
            else if (obstacleType == 4 && hasCoin)
                obstacleType = rand() % (TypeCount - 2);
            
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
            tmpAnimation.setPosition(mObstacleQueue.back().second.getPosition().x + delta, mGroundHeight);
            // switch (obstacleType)
            // {
            // case 0:
            //     tmpTexture = *getContext().mTextures->get(Textures::Milktea);
            //     break;
            // case 1:
            //     tmpTexture = *getContext().mTextures->get(Textures::Cat);
            //     break;
            // case 2:
            //     tmpTexture = *getContext().mTextures->get(Textures::Friend);
            //     break;
            // case 3:
            //     tmpTexture = *getContext().mTextures->get(Textures::Rock);
            //     break;
            // case 4:
            //     tmpTexture = *getContext().mTextures->get(Textures::Coin);
            //     hasCoin = true;
            //     break;
            // case 5:
            //     tmpTexture = *getContext().mTextures->get(Textures::Heart);
            //     hasHeart = true;
            //     break;
            // default:
            //     break;
            // }
            sf::Texture &tmpTexture = getContext().mTextures->get(Textures::Heart);
            tmpAnimation.setTexture(tmpTexture);
            int width = tmpTexture.getSize().x / 1;
            int height = tmpTexture.getSize().y / 2;

            tmpAnimation.setFrameSize(sf::Vector2i(width, height));
            tmpAnimation.setNumFrames(2);
            tmpAnimation.setDuration(sf::seconds(1.f));
            tmpAnimation.centerOrigin();
            tmpAnimation.setRepeating(true);
            mObstacleQueue.push_back(std::make_pair(obstacleType, tmpAnimation));
        }
    }
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
        mDistanceText.setString("Distance: " + std::to_string((int)mDistance / 100) + "m");
        if (!mObstacleQueue.empty() && mObstacleQueue.front().second.getPosition().x < -mObstacleQueue.front().second.getFrameSize().x)
            mObstacleQueue.pop_front();
        for (auto &obstacle : mObstacleQueue)
        {
            obstacle.second.update(dt);
            obstacle.second.move(-mScrollSpeed * dt.asSeconds(), 0.f);
        }
        createObstacle();

        for (auto &obstacle : mObstacleQueue)
        {
            if (Collision::pixelPerfectTest(mPlayer.getSprite(), obstacle.second.getSprite(), (sf::Uint8)0U))
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
    /*
    // this code will cause unexpected behavior
    for (int i = 0; i < 2; i++)
    {
        target.draw(mBackgroundSprite[i], states);
        target.draw(mGroundSprite[i], states);
    }
    */
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
        target.draw(obstacle.second, states);
    }
    target.draw(mPlayer, states);
}   