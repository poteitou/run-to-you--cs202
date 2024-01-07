#include <MINE/EndState.hpp>

EndState::EndState(StateStack &stack, Context context)
    : State(stack, context),
      mBackgroundSprite(),
      mGroundSprite(),
      mLives(context.mTextures->get(Textures::Life)),
      mDistanceText("", context.mFonts->get(Fonts::Main), 50),
      mPlayer(context),
      mNonPlayer(context, false),
      mGroundHeight(900.f - 80.f),
      mScrollSpeed(800.f),
      mTimeCollide(1.f),
      mDistance(64000.f),
      mIsPaused(true)
{
    mCntLives = getCount();
    sf::Texture &backgroundTexture = context.mTextures->get(Textures::BlueBG);
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

    mLives.setTextureRect(sf::IntRect(0, mCntLives * 100, 320, 100));
    mLives.setPosition(1600.f - 320.f - 50.f, 50.f);

    mPaused.setBuffer(context.mSoundBuffers->get(Sounds::Paused));
    mPaused.setVolume(100);
    mGameOver.setBuffer(context.mSoundBuffers->get(Sounds::GameOver));
    mGameOver.setVolume(100);
    mCollide.setBuffer(context.mSoundBuffers->get(Sounds::Collide));
    mCollide.setVolume(100);

    mPlayer.setPosition(150.f, 300.f);
    mPlayer.setVelocity(0.f, 0.f);
    mNonPlayer.setPosition(120.f, 300.f);
    mNonPlayer.setVelocity(0.f, 0.f);
    
    // prepare for distance text
    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);

    mTypeObject = {"Tree", "Rock", "RockHeap", "Bird1", "Bird2", "Heart1", "Heart2", "Heart3"};

    int obstacleType = randomInt(1, mTypeObject.size() - 3) - 1;

    mObstacleQueue.push_back(Object(context, "Heart1", 2400.f, mGroundHeight));

    if (!mMusic.openFromFile("resources/sounds/MediumMusic.ogg"))
        throw std::runtime_error("Music MediumMusic could not be loaded.");
    if (playMusic())
        mMusic.setVolume(40);
    else
        mMusic.setVolume(0);
        
    mMusic.setLoop(true);
}

int EndState::randomInt(int l, int r)
{
    std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());
    return l + rd() % (r - l + 1);
}

void EndState::createObstacle()
{
    if (!mObstacleQueue.empty() && mObstacleQueue.back().getPosition().x < 1600.f)
    {
        bool hasHeart = false;

        while (mObstacleQueue.back().getPosition().x < 6400.f)
        {
            int obstacleType = randomInt(1, mTypeObject.size() - 3 * hasHeart) - 1;
            if (mTypeObject[obstacleType] == "Heart1" || mTypeObject[obstacleType] == "Heart2" || mTypeObject[obstacleType] == "Heart3")
                hasHeart = true;

            int randPos = randomInt(0, 5);
            float minDis = mScrollSpeed * 1.f;
            float maxDis = mScrollSpeed * 1.8f;
            float delta = minDis + (maxDis - minDis) * randPos / 5;
            float pos = mObstacleQueue.back().getPosition().x + delta;
            if (mTypeObject[obstacleType] == "Bird1")
                mObstacleQueue.push_back(Object(getContext(), "Bird3", pos, mGroundHeight));
            if (mTypeObject[obstacleType] == "Heart2")
                mObstacleQueue.push_back(Object(getContext(), "Rock", pos, mGroundHeight));
            if (mTypeObject[obstacleType] == "Heart3")
                mObstacleQueue.push_back(Object(getContext(), "RockHeap", pos, mGroundHeight));
            mObstacleQueue.push_back(Object(getContext(), mTypeObject[obstacleType], pos, mGroundHeight));
        }
    }
}

void EndState::recordScore()
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
    while (scores.size() > 3)
        scores.pop_back();
    std::ofstream fout("resources/texts/highscore.data");
    for (int i = 0; i < 3; i++)
        fout << scores[i] << std::endl;
    fout.close();
}

bool EndState::handleEvent(User user)
{
    if (user.isEscapePressed)
    {
        mIsPaused = true;
        mMusic.pause();
        mPaused.play();
        requestStackPush(States::Paused);
    }
    mPlayer.handleEvent(user);
    mNonPlayer.handleEvent(user);
    return true;
}

bool EndState::update(sf::Time dt)
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
            --mCntLives;
            mLives.setTextureRect(sf::IntRect(0, mCntLives * 100, 320, 100));
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
            mLives.setTextureRect(sf::IntRect(0, mCntLives * 100, 320, 100));
        }
        else if (obstacle.isCollide() && (obstacle.getType() != "Heart1" && obstacle.getType() != "Heart2" && obstacle.getType() != "Heart3"))
        {
            mPlayer.update(dt, 0);
            mNonPlayer.update(dt, -1.f);
            mTimeCollide = 0.f;
            obstacle.update(dt, 0.f, mPlayer);
            return true;
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
    mNonPlayer.update(dt, mGroundHeight);
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
    if (mScrollSpeed < 3000.f)
        mScrollSpeed += 0.1f;
    return true;
}

void EndState::draw(sf::RenderTarget &target, sf::RenderStates states) const
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
    target.draw(mNonPlayer, states);
    target.draw(mPlayer, states);
}