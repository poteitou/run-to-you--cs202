#include <MINE/BeginState.hpp>

BeginState::BeginState(StateStack &stack, Context context)
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
    mCrush.setPosition(36000.f + 900.f, mGroundHeight);

    // prepare for distance text
    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);

    mTypeObject = {"Tree", "Rock", "Bird", "Heart"};
    // mTypeObject[1] = {"Milktea", "Dog", "Tree", "Rock", "Bird", "Heart"};

    srand(time(NULL));
    int obstacleType = rand() % (mTypeObject.size() - 1);

    mObstacleQueue.push_back(Object(context, mTypeObject[obstacleType], 2400.f, mGroundHeight));

    if (!mMusic.openFromFile("resources/sounds/Forest.ogg"))
        throw std::runtime_error("Music Forest could not be loaded.");
    if (playMusic())
        mMusic.setVolume(30);
    else
        mMusic.setVolume(0);
        
    mMusic.setLoop(true);
    mMusic.play();
}

void BeginState::createObstacle()
{
    srand(time(NULL));
    if (((int)mDistance / 100) > 320.f)
        return;
    // if (((int)mDistance / 100) > 910.f)
    //     return;
    if (!mObstacleQueue.empty() && mObstacleQueue.back().getPosition().x < 1600.f)
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

            int randPos = rand() % 5;
            float minDis = mScrollSpeed * 0.8f;
            float maxDis = mScrollSpeed * 1.6f;
            float delta = minDis + (maxDis - minDis) * randPos / 5;
            mObstacleQueue.push_back(Object(getContext(), mTypeObject[obstacleType], mObstacleQueue.back().getPosition().x + delta, mGroundHeight));
        }
    }
}

void BeginState::recordScore()
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

bool BeginState::handleEvent(User user)
{
    if (user.isEscapePressed)
    {
        mIsPaused = true;
        mMusic.pause();
        mPaused.play();
        requestStackPush(States::Paused);
    }
    mPlayer.handleEvent(user);
    return true;
}

bool BeginState::update(sf::Time dt)
{
    if (mIsPaused)
    {
        mMusic.play();
        mIsPaused = false;
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

    if (((int)mDistance / 100) > 360.f)
    {
        mDistance = 36000.f;
        setCount(mCntLives);
        requestStackPop();
        requestStackPush(States::BTM);
    }
    // if (((int)mDistance / 100) > 1000.f)
    // {
    //     mDistance = 100000.f;
    //     mStartLevelUp = mLevelingUp = true;
    // }
    mDistanceText.setString("Distance: " + std::to_string((int)mDistance / 100) + "m");
    if (!mObstacleQueue.empty() && mObstacleQueue.front().isOutOfScreen())
        mObstacleQueue.pop_front();
    for (auto &obstacle : mObstacleQueue)
    {
        obstacle.update(dt, mScrollSpeed, mPlayer);
    }
    createObstacle();

    mPlayer.update(dt, mGroundHeight);
    mCrush.update(dt);
    mCrush.move(-mScrollSpeed * dt.asSeconds(), 0.f);
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

void BeginState::draw(sf::RenderTarget &target, sf::RenderStates states) const
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