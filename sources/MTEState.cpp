#include <MINE/MTEState.hpp>

MTEState::MTEState(StateStack &stack, Context context)
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

    mBackgroundSprite.setTexture(context.mTextures->get(Textures::OrangeBG));
    mBackgroundSprite.setPosition(0.f, 0.f);
    mGroundSprite.setTexture(context.mTextures->get(Textures::Ground));
    mGroundSprite.setPosition(0.f, 0.f);

    mChange.setTexture(context.mTextures->get(Textures::OrangeToBlue));
    mChange.setFrameSize(sf::Vector2i(1600, 900));
    mChange.setNumFrames(8);
    mChange.setDuration(sf::seconds(2.f));
    mChange.setRepeating(false);
    mChange.setPosition(0.f, 0.f);

    mLives.setTextureRect(sf::IntRect(0, getCount() * 100, 320, 100));
    mLives.setPosition(1600.f - 320.f - 50.f, 50.f);

    mDistanceText.setPosition(50.f, 80.f);
    mDistanceText.setColor(sf::Color::Black);
    mDistanceText.setString("Distance: 640m");

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

bool MTEState::handleEvent(User user)
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

bool MTEState::update(sf::Time dt)
{
    if (mShowText > 1)
        mButton->update(dt);
    if (mButton->isPressed())
    {
        mShowText = 20;
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
            mRect[0].setSize(sf::Vector2f(450.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setFillColor(sf::Color::White);
            mRect[0].setOutlineColor(sf::Color::Black);
            mRect[0].setOutlineThickness(5.f);

            mRect[1].setSize(sf::Vector2f(200.f, 5.f));
            mRect[1].setFillColor(sf::Color::Black);

            mText[0].setColor(sf::Color::Black);
            mText[0].setString("Hi! We meet again :D");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 5) // Hi! We meet again :D
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 6;
            mRect[2].setSize(sf::Vector2f(200.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setFillColor(sf::Color::White);
            mRect[2].setOutlineColor(sf::Color::Black);
            mRect[2].setOutlineThickness(5.f);

            mRect[3].setSize(sf::Vector2f(200.f, 5.f));
            mRect[3].setFillColor(sf::Color::Black);

            mText[1].setColor(sf::Color::Black);
            mText[1].setString("Yeah :D");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 6) // Yeah :>
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
            mRect[0].setSize(sf::Vector2f(650.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setFillColor(sf::Color::White);
            mRect[0].setOutlineColor(sf::Color::Black);
            mRect[0].setOutlineThickness(5.f);

            mRect[1].setSize(sf::Vector2f(200.f, 5.f));
            mRect[1].setFillColor(sf::Color::Black);

            mText[0].setColor(sf::Color::Black);
            mText[0].setString("I see that you like running");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 8) // I see that you like running
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 9;
            mRect[2].setSize(sf::Vector2f(200.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("Yes...");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 9) // Yes...
    {
        mTime += dt.asSeconds();
        if (mTime > 2.5f)
        {
            mTime = 0.f;
            mShowText = 10;
            mRect[2].setSize(sf::Vector2f(350.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("And I like you");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 10) // And I like you
    {
        mTime += dt.asSeconds();
        if (mTime > 1.f)
        {
            mTime = 0.f;
            mShowText = 11;
        }
    }
    else if (mShowText == 11) // Silence
    {
        mTime += dt.asSeconds();
        if (mTime > 0.5f)
        {
            mTime = 0.f;
            mShowText = 12;
            mRect[0].setSize(sf::Vector2f(450.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);

            mText[0].setString("What did you say?");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 12) // What did you say?
    {
        mTime += dt.asSeconds();
        if (mTime > 0.5f)
        {
            mTime = 0.f;
            mShowText = 13;
            mRect[2].setSize(sf::Vector2f(250.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("N-Nothing");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 13) // :') Nothing
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 14;
        }
    }
    else if (mShowText == 14) // Silence
    {
        mTime += dt.asSeconds();
        if (mTime > 0.2f)
        {
            mTime = 0.f;
            mShowText = 15;
            mRect[0].setSize(sf::Vector2f(350.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setFillColor(sf::Color::White);
            mRect[0].setOutlineColor(sf::Color::Black);
            mRect[0].setOutlineThickness(5.f);

            mRect[1].setSize(sf::Vector2f(200.f, 5.f));
            mRect[1].setFillColor(sf::Color::Black);

            mText[0].setColor(sf::Color::Black);
            mText[0].setString("I like you too");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 15) // I like you too :>
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 16;
            mRect[0].setSize(sf::Vector2f(500.f, 100.f));
            mRect[0].setOrigin(0.5f * mRect[0].getLocalBounds().width, 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setPosition(mCrush.getPosition().x + 150.f, mCrush.getPosition().y - mCrush.getFrameSize().y - 75.f - 0.5f * mRect[0].getLocalBounds().height);
            mRect[0].setFillColor(sf::Color::White);
            mRect[0].setOutlineColor(sf::Color::Black);
            mRect[0].setOutlineThickness(5.f);

            mRect[1].setSize(sf::Vector2f(200.f, 5.f));
            mRect[1].setFillColor(sf::Color::Black);

            mText[0].setColor(sf::Color::Black);
            mText[0].setString("Wanna run together?");
            mText[0].setOrigin(0.5f * mText[0].getLocalBounds().width, 0.5f * mText[0].getLocalBounds().height);
            mText[0].setPosition(mRect[0].getPosition().x, mRect[0].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 16) // Wanna run together?
    {
        mTime += dt.asSeconds();
        if (mTime > 0.8f)
        {
            mTime = 0.f;
            mShowText = 17;
            mRect[2].setSize(sf::Vector2f(300.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("What...? O_O");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 17) // What...? O_O
    {
        mTime += dt.asSeconds();
        if (mTime > 2.5f)
        {
            mTime = 0.f;
            mShowText = 18;
            mRect[2].setSize(sf::Vector2f(300.f, 100.f));
            mRect[2].setOrigin(0.5f * mRect[2].getLocalBounds().width, 0.5f * mRect[2].getLocalBounds().height);
            mRect[2].setPosition(mGirl.getPosition().x - 150.f, mGirl.getPosition().y - mGirl.getFrameSize().y - 75.f - 0.5f * mRect[2].getLocalBounds().height);

            mText[1].setString("O-okay O_O");
            mText[1].setOrigin(0.5f * mText[1].getLocalBounds().width, 0.5f * mText[1].getLocalBounds().height);
            mText[1].setPosition(mRect[2].getPosition().x, mRect[2].getPosition().y - 25.f);

            mTextSound.play();
            mTime += dt.asSeconds();
        }
    }
    else if (mShowText == 18) // O-okayy ><
    {
        mTime += dt.asSeconds();
        if (mTime > 2.f)
        {
            mTime = 0.f;
            mShowText = 19;
        }
    }
    else if (mShowText == 19) // Silence
    {
        mTime += dt.asSeconds();
        if (mTime > 2.5f)
        {
            mTime = 0.f;
            mShowText = 20;
        }
    }
    else if (mShowText == 20)
    {
        mChange.update(dt);
        if (mChange.isFinished())
        {
            requestStackPop();
            requestStackPop();
            requestStackPush(States::End);
        }
    }

    return false;
}

void MTEState::draw(sf::RenderTarget &target, sf::RenderStates states) const
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
    else if (mShowText == 5 || mShowText == 8 || mShowText == 12 || mShowText == 15 || mShowText == 16)
    {
        sf::Transform transform;
        transform.translate(sf::Vector2f(mCrush.getPosition().x + 50.f, mCrush.getPosition().y - mCrush.getFrameSize().y));
        transform.rotate(-35.f);
        target.draw(mRect[1], transform);
        target.draw(mRect[0], states);
        target.draw(mText[0], states);
    }
    else if (mShowText == 6 || mShowText == 9 || mShowText == 10 || mShowText == 13 || mShowText == 17 || mShowText == 18)
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
    else if (mShowText == 20)
    {
        target.draw(mChange, states);
    }
}
