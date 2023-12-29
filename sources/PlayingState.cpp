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
      mTimeCollide(1.f),
      mDistance(0.f),
      mCntLives(1),
      mLevel(0),
      mStartLevelUp(false),
      mLevelingUp(false),
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
    mCollide.setBuffer(context.mSoundBuffers->get(Sounds::Collide));
    mCollide.setVolume(100);

    mPlayer.setPosition(150.f, 300.f);
    mPlayer.setVelocity(0.f, 0.f);

    int mWidth = context.mTextures->get(Textures::CrushRight).getSize().x / 9;
    int mHeight = context.mTextures->get(Textures::CrushRight).getSize().y;
    mCrush.setTexture(context.mTextures->get(Textures::CrushRight));
    mCrush.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mCrush.setNumFrames(8);
    mCrush.setDuration(sf::seconds(0.8f));
    mCrush.centerBottom();
    mCrush.setRepeating(true);
    mCrush.setPosition(50000.f, mGroundHeight);

    // prepare for distance text
    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);

    // initialize obstacle queue, include some obstacles start from position 2000.f
    mTypeObject[0] = {"Milktea", "Dog", "Friend", "Rock", "Bird", "Heart"};
    mTypeObject[1] = {"Milktea", "Dog", "Friend", "Rock", "Bird", "Heart"};
    mTypeObject[2] = {"Milktea", "Dog", "Friend", "Rock", "Bird", "Heart"};

    srand(time(NULL));
    int obstacleType = rand() % (mTypeObject[0].size() - 1);

    mObstacleQueue.push_back(Object(context, mTypeObject[0][obstacleType], 2400.f, mGroundHeight));

    if (!mMusic.openFromFile("resources/sounds/Forest.ogg"))
        throw std::runtime_error("Music Forest could not be loaded.");
    mMusic.setVolume(30);
    mMusic.setLoop(true);
    mMusic.play();
}

void PlayingState::createObstacle()
{
    srand(time(NULL));
    switch (mLevel)
    {
    case 0:
        if (((int)mDistance / 100) > 410.f)
            return;
    case 1:
        if (((int)mDistance / 100) > 910.f)
            return;
    default:
        break;
    }
    if (!mObstacleQueue.empty() && mObstacleQueue.back().getPosition().x < 1600.f)
    {
        bool hasHeart = false;
        bool hasCoin = false;

        while (mObstacleQueue.back().getPosition().x < 3200.f)
        {
            int obstacleType = rand() % mTypeObject[mLevel].size();
            if (mTypeObject[mLevel][obstacleType] == "Heart")
            {
                if (hasHeart)
                    obstacleType = rand() % (mTypeObject[mLevel].size() - 1);
                hasHeart = true;
            }

            int randPos = rand() % 5;
            float minDis = mScrollSpeed * 0.8f;
            float maxDis = mScrollSpeed * 1.6f;
            float delta = minDis + (maxDis - minDis) * randPos / 5;
            mObstacleQueue.push_back(Object(getContext(), mTypeObject[mLevel][obstacleType], mObstacleQueue.back().getPosition().x + delta, mGroundHeight));
        }
    }
}

void PlayingState::recordScore()
{
    std::ifstream fin("resources/texts/highscore.data");
    std::vector<int> scores;
    int score;
    while (fin >> score)
    {
        scores.push_back(score);
    }
    fin.close();
    scores.push_back((int)mDistance / 100);
    std::sort(scores.begin(), scores.end(), std::greater<int>());
    std::ofstream fout("resources/texts/highscore.data");
    for (int i = 0; i < scores.size() && i < 10; i++)
    {
        fout << scores[i] << std::endl;
        // fout << std::to_string(i + 1) << ". " << scores[i] << " meters" << std::endl;
    }
    fout.close();
}

void PlayingState::levelUp(sf::Time dt)
{
    if (mStartLevelUp)
    {
        mStartLevelUp = false;
        mObstacleQueue.clear();
        mPlayer.setVelocity(400.f, 0.f);
    }
    
    mPlayer.update(dt, mGroundHeight);
    if (mPlayer.getPosition().x > 1600.f / 2 - mPlayer.getGlobalBounds().width / 2)
    {
        sf::Texture &backgroundTexture = mLevel == 1 ? getContext().mTextures->get(Textures::OrangeSky) : getContext().mTextures->get(Textures::CityNight);
        sf::Texture &groundTexture = getContext().mTextures->get(Textures::Ground);

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

        mPlayer.setPosition(150.f, mGroundHeight);
        mPlayer.setVelocity(0.f, 0.f);

        srand(time(NULL));
        int obstacleType = rand() % (mTypeObject[mLevel].size() - 1);
        mObstacleQueue.push_back(Object(getContext(), mTypeObject[0][obstacleType], 2400.f, mGroundHeight));
        mLevelingUp = false;
    }
}

bool PlayingState::handleEvent(User user)
{
    if (user.isEscapePressed)
    {
        mIsPaused = true;
        mMusic.pause();
        mPaused.play();
        requestStackPush(States::Paused);
    }
    else if (mIsPaused)
    {
        mMusic.play();
        mIsPaused = false;
    }
    if (!mLevelingUp)
        mPlayer.handleEvent(user);
    return true;
}

bool PlayingState::update(sf::Time dt)
{
    if (mLevelingUp)
    {
        levelUp(dt);
        return true;
    }
    if (mTimeCollide < 0.5f)
    {
        mTimeCollide += dt.asSeconds();
        if (mTimeCollide >= 0.5f)
        {
            // --mCntLives;
            mLives.setTextureRect(sf::IntRect(0, mCntLives * 80, 256, 80));
            if (mCntLives == 0)
            {
                recordScore();
                mMusic.stop();
                mGameOver.play();
                requestStackPush(States::GameOver);
            }
        }
        return true;
    }
    for (auto &obstacle : mObstacleQueue)
    {
        if (obstacle.isDone() && mCntLives < 3)
        {
            ++mCntLives;
            mLives.setTextureRect(sf::IntRect(0, mCntLives * 80, 256, 80));
        }
        // else if (obstacle.isCollide() && obstacle.getType() != "Heart")
        // {
        //     mPlayer.update(dt, 0);
        //     mTimeCollide = 0.f;
        //     obstacle.update(dt, 0.f, mPlayer);
        //     return true;
        // }
    }

    mDistance += mScrollSpeed * dt.asSeconds();
    switch (mLevel)
    {
    case 0:
        if (((int)mDistance / 100) > 500.f)
        {
            mDistance = 50000.f;
            mStartLevelUp = mLevelingUp = true;
            mLevel = 1;
        }
    case 1:
        if (((int)mDistance / 100) > 1000.f)
        {
            mDistance = 100000.f;
            mStartLevelUp = mLevelingUp = true;
            mLevel = 2;
        }
    default:
        break;
    }
    mDistanceText.setString("Distance: " + std::to_string((int)mDistance / 100) + "m");
    if (!mObstacleQueue.empty() && mObstacleQueue.front().isOutOfScreen())
        mObstacleQueue.pop_front();
    for (auto &obstacle : mObstacleQueue)
    {
        obstacle.update(dt, mScrollSpeed, mPlayer);
    }
    createObstacle();

    if (mLevel < 2)
    {
        mCrush.update(dt);
        mCrush.move(-mScrollSpeed * dt.asSeconds(), 0.f);
    }
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
        mScrollSpeed += 0.1f;
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
    target.draw(mCrush, states);
}