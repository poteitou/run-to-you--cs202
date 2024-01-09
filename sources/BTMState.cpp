#include <MINE/BTMState.hpp>

BTMState::BTMState(StateStack &stack, Context context)
    : State(stack, context),
      mGirl(context.mTextures->get(Textures::BlueSkirt)),
      mCrush(context.mTextures->get(Textures::CrushRight)),
      mLives(context.mTextures->get(Textures::Life)),
      mDistanceText("", context.mFonts->get(Fonts::Main), 50),
      mTextSound(context.mSoundBuffers->get(Sounds::Button)),
      mShowText(0)
{
    mTextSound.setVolume(100);

    int mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
    int mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 3;
    mGirl.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mGirl.setNumFrames(8);
    mGirl.setDuration(sf::seconds(0.8f));
    mGirl.centerBottom();
    mGirl.setRepeating(true);
    mGirl.setPosition(150.f, 900.f - 80.f);

    mGirl.setFrame(9);

    mWidth = context.mTextures->get(Textures::CrushRight).getSize().x / 9;
    mHeight = context.mTextures->get(Textures::CrushRight).getSize().y;
    mCrush.setTexture(context.mTextures->get(Textures::CrushRight));
    mCrush.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mCrush.setNumFrames(8);
    mCrush.setDuration(sf::seconds(0.8f));
    mCrush.centerBottom();
    mCrush.setRepeating(true);
    mCrush.setPosition(900.f, 900.f - 80.f);

    mCrush.setFrame(8);
    mTime = 0.f;

    mBackgroundSprite.setTexture(context.mTextures->get(Textures::PinkBG));
    mBackgroundSprite.setPosition(0.f, 0.f);
    mGroundSprite.setTexture(context.mTextures->get(Textures::Ground));
    mGroundSprite.setPosition(0.f, 0.f);

    mChange.setTexture(context.mTextures->get(Textures::PinkToOrange));
    mChange.setFrameSize(sf::Vector2i(1600, 900));
    mChange.setNumFrames(12);
    mChange.setDuration(sf::seconds(3.f));
    mChange.setRepeating(false);
    mChange.setPosition(0.f, 0.f);

    mLives.setTextureRect(sf::IntRect(0, getCount() * 100, 320, 100));
    mLives.setPosition(1600.f - 320.f - 50.f, 50.f);

    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);
    mDistanceText.setString("Distance: 320m");

    mText[0].setFont(context.mFonts->get(Fonts::Main));
    mText[1].setFont(context.mFonts->get(Fonts::Main));
    mText[0].setString("Click anywhere to continue");
    mText[0].setCharacterSize(60);
    mText[1].setCharacterSize(50);
    mText[0].setColor(sf::Color::Black);
    mText[0].setPosition(0.5f * 1600.f - 0.5f * mText[0].getLocalBounds().width, 0.25f * 900.f - 0.5f * mText[0].getLocalBounds().height);

    mButton = std::make_shared<Button>(context, Textures::ButtonSkip);
    mButton->setPosition(1600.f - 0.5f * mButton->getLocalBounds().width - 50.f, 900.f - 0.5f * mButton->getLocalBounds().height - 50.f);
}

bool BTMState::handleEvent(User user)
{   
    if (mShowText <= 1 && user.isMousePressed)
    {
        mText[0].setCharacterSize(50);
        mShowText = 2;
        mTime = 0.f;
    }
    else if (mShowText > 1)
    {
        mButton->handleEvent(user);
    }
    return false;
}

bool BTMState::update(sf::Time dt)
{
    if (mShowText > 1)
        mButton->update(dt);
    if (mButton->isPressed())
    {
        mShowText = 15;
        mTime = 0.f;
    }
    if (mShowText <= 1) // not click
    {
        mTime += dt.asSeconds();
        if (mTime >= 0.5f)
        {
            mShowText = 1 - mShowText;
            mTime = 0.f;
        }
    }
    else if (mShowText == 2) // after click 0.5f
    {
        mTime += dt.asSeconds();
        if (mTime > 1.8f)
        {
            mTime = 0.f;
            mGirl.update(dt);
            mGirl.move(200.f * dt.asSeconds(), 0.f);
            mShowText = 3;
        }
    }
    else if (mShowText == 3) // moving to 700.f
    {
        mGirl.update(dt);
        mGirl.move(200.f * dt.asSeconds(), 0.f);
        if (mGirl.getPosition().x >= 700.f)
        {
            mShowText = 4;
            mGirl.setFrame(9);
        }
    }
    else if (mShowText == 4) // after 700.f
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 5;
            mRect[0].setSize(sf::Vector2f(300.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setFillColor(sf::Color::White);
            mRect[0].setOutlineColor(sf::Color::Black);
            mRect[0].setOutlineThickness(5.f);

            mRect[1].setSize(sf::Vector2f(200.f, 5.f));
            mRect[1].setFillColor(sf::Color::Black);

            mText[0].setColor(sf::Color::Black);
            mText[0].setString("Oh, Hello! :D");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 5) // Hello
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 6;
            mRect[2].setSize(sf::Vector2f(250.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setFillColor(sf::Color::White);
            mRect[2].setOutlineColor(sf::Color::Black);
            mRect[2].setOutlineThickness(5.f);

            mRect[3].setSize(sf::Vector2f(200.f, 5.f));
            mRect[3].setFillColor(sf::Color::Black);

            mText[1].setColor(sf::Color::Black);
            mText[1].setString("H-Hi! ><");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 6) // H-Hi
    {
        mTime += dt.asSeconds();
        if (mTime > 1.5f)
        {
            mTime = 0.f;
            mShowText = 7;
        }
    }
    else if (mShowText == 7) // Silence
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 8;
            mRect[0].setSize(sf::Vector2f(450.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setFillColor(sf::Color::White);
            mRect[0].setOutlineColor(sf::Color::Black);
            mRect[0].setOutlineThickness(5.f);

            mRect[1].setSize(sf::Vector2f(200.f, 5.f));
            mRect[1].setFillColor(sf::Color::Black);

            mText[0].setColor(sf::Color::Black);
            mText[0].setString("What a coincidence!");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 8) // What a coincidence!
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 9;
            mRect[2].setSize(sf::Vector2f(200.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("Y-yah...");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 9) // Y-yah
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 10;
            mRect[2].setSize(sf::Vector2f(300.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("I guess so :))");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 10) // I guess so :))
    {
        mTime += dt.asSeconds();
        if (mTime > 1.5f)
        {
            mTime = 0.f;
            mShowText = 11;
        }
    }
    else if (mShowText == 11) // Silence
    {
        mTime += dt.asSeconds();
        if (mTime > 1.5f)
        {
            mTime = 0.f;
            mShowText = 12;
            mRect[0].setSize(sf::Vector2f(600.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);

            mText[0].setString("I have to go. See u later :D");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 12) // I have to go. See u later :D
    {
        mTime += dt.asSeconds();
        if (mTime > 1.5f)
        {
            mTime = 0.f;
            mShowText = 13;
            mRect[2].setSize(sf::Vector2f(250.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("Oh, okay...");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 13) // Oh, okay...
    {
        mTime += dt.asSeconds();
        if (mTime > 2.5f)
        {
            mTime = 0.f;
            mShowText = 14;
        }
    }
    else if (mShowText == 14) // Silence
    {
        mTime += dt.asSeconds();
        if (mTime > 2.5f)
        {
            mTime = 0.f;
            mShowText = 15;
        }
    }
    else if (mShowText == 15)
    {
        mChange.update(dt);
        if (mChange.isFinished())
        {
            requestStackPop();
            requestStackPop();
            requestStackPush(States::Middle);
        }
    }

    return false;
}

void BTMState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mBackgroundSprite, states);
    target.draw(mGroundSprite, states);
    target.draw(mGirl, states);
    target.draw(mCrush, states);
    target.draw(mLives, states);
    target.draw(mDistanceText, states);
    if (mShowText > 1)
        target.draw(*mButton, states);

    if (mShowText == 1)
        target.draw(mText[0], states);
    else if (mShowText == 5 || mShowText == 8 || mShowText == 12)
    {
        sf::Transform transform;
        transform.translate(sf::Vector2f(mCrush.getPosition().x + 50.f, mCrush.getPosition().y - mCrush.getFrameSize().y));
        transform.rotate(-35.f);
        target.draw(mRect[1], transform);
        target.draw(mRect[0], states);
        target.draw(mText[0], states);
    }
    else if (mShowText == 6 || mShowText == 9 || mShowText == 10 || mShowText == 13)
    {
        sf::Transform transform;
        transform.translate(sf::Vector2f(mCrush.getPosition().x + 50.f, mCrush.getPosition().y - mCrush.getFrameSize().y));
        transform.rotate(-35.f);
        target.draw(mRect[1], transform);
        target.draw(mRect[0], states);
        target.draw(mText[0], states);

        transform = sf::Transform();
        transform.translate(sf::Vector2f(mGirl.getPosition().x - 50.f, mGirl.getPosition().y - mGirl.getFrameSize().y));
        transform.rotate(180.f + 35.f);
        target.draw(mRect[3], transform);
        target.draw(mRect[2], states);
        target.draw(mText[1], states);
    }
    else if (mShowText == 15)
    {
        target.draw(mChange, states);
    }
}
