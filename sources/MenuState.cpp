#include <MINE/MenuState.hpp>
#include <iostream>

MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context), 
      mBackgroundSprite(), 
      mButtons(), 
      mTitle("RUN", context.mFonts->get(Fonts::Caro), 300),
      mTitleSprite(context.mTextures->get(Textures::ToYou)),
      mGirl(context.mTextures->get(Textures::BlueSkirt)),
      mDrawPopup(0)
{
    mTitle.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color(63, 105, 195));
    mTitle.setPosition(0.5f * 1600.f - 180.f, 0.3 * 900.f);

    int mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
    int mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 3;
    mGirl.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mGirl.setNumFrames(8);
    mGirl.setDuration(sf::seconds(0.8f));
    mGirl.centerBottom();
    mGirl.setRepeating(true);
    // mGirl is on the left of mTitle
    mGirl.setPosition(0.5f * 1600.f - 180.f - 2.3f * mWidth, 0.3 * 900.f + 0.5f * mHeight);

    mTitleSprite.setPosition(0.5f * 1600.f, 0.35 * 900.f);

    mBackgroundSprite.setTexture(context.mTextures->get(Textures::MenuBG));
    mBackgroundSprite.setPosition(0.f, 0.f);

    mGroundSprite.setTexture(context.mTextures->get(Textures::GroundMenu));
    mGroundSprite.setPosition(0.f, 0.f);

    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));

    // small rect when popup will scroll in the right
    mScrollRect.setSize(sf::Vector2f(50.f, 0.1f * 900.f));
    mScrollRect.setFillColor(sf::Color(31, 65, 114));

    mButtons[0] = std::make_shared<Button>(context, Textures::ButtonPlay);
    mButtons[0]->setText("   Play", 100);
    // button on the right botton of the screen
    mButtons[0]->setPosition(1600.f - 25.f - 0.5f * mButtons[0]->getLocalBounds().width , 0.75f * 900.f + 0.5f * mButtons[0]->getLocalBounds().height - 75.f);

    mButtons[1] = std::make_shared<Button>(context, Textures::ButtonAbout);
    // button on the left of Play button[0]
    mButtons[1]->setPosition(1600.f - 25.f - 0.5f * mButtons[1]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 25.f, 0.75f * 900.f + 0.5f * mButtons[1]->getLocalBounds().height);

    mButtons[2] = std::make_shared<Button>(context, Textures::ButtonRank);
    // button on the left of button[1]
    mButtons[2]->setPosition(1600.f - 25.f - 0.5f * mButtons[2]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 200.f, 0.75f * 900.f + 0.5f * mButtons[2]->getLocalBounds().height);

    mButtons[3] = std::make_shared<Button>(context, Textures::ButtonMusic);
    // button on the left of button[2]
    mButtons[3]->setPosition(1600.f - 25.f - 0.5f * mButtons[3]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 375.f, 0.75f * 900.f + 0.5f * mButtons[3]->getLocalBounds().height);

    mButtons[4] = std::make_shared<Button>(context, Textures::ButtonX);
    mButtons[5] = std::make_shared<Button>(context, Textures::Button);
    mButtons[5]->setText("Reset", 70);
    mButtons[5]->setPosition(0.5f * 1600.f, 787.5f - 0.5f * mButtons[5]->getLocalBounds().height - 25.f);
    mButtons[6] = std::make_shared<Button>(context, Textures::Button);
    mButtons[6]->setText("OK", 70);
    mButtons[6]->setPosition(0.5f * 1600.f - 0.5 * 450.f, 631.f);
    mButtons[7] = std::make_shared<Button>(context, Textures::Button);
    mButtons[7]->setText("New game", 70);
    mButtons[7]->setPosition(0.5f * 1600.f + 0.5 * 450.f, 631.f);
    mRank.setTexture(context.mTextures->get(Textures::Rank));
    mRank.setPosition(0.5f * 1600.f - 0.5f * mRank.getLocalBounds().width, 0.5f * 900.f - 0.5f * mRank.getLocalBounds().height - 25.f);
    mPopup.setTexture(context.mTextures->get(Textures::Popup));
    mPopupRect.setTexture(context.mTextures->get(Textures::PopupRect));

    for (int i = 0; i < 6; i++)
        mButtonPressed[i] = false;

    if (!mMusic.openFromFile("resources/sounds/FastMusic.ogg"))
		throw std::runtime_error("Music FastMusic could not be loaded.");
    
    if (playMusic())
        mMusic.setVolume(70);
    else
    {
        mMusic.setVolume(0);
        mButtons[3]->setTexture(getContext().mTextures->get(Textures::ButtonMute));
    }
    mMusic.setLoop(true);
    mMusic.play();

    mBirdGreen.setTexture(context.mTextures->get(Textures::BirdGreen));
    mWidth = mBirdGreen.getTexture()->getSize().x / 8;
    mHeight = mBirdGreen.getTexture()->getSize().y;
    mBirdGreen.setNumFrames(8);
    mBirdGreen.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mBirdGreen.setDuration(sf::seconds(0.7f));
    mBirdGreen.centerBottom();
    mBirdGreen.setRepeating(true);
    mBirdGreen.setPosition(400.f, 900.f - 80.f - 75.f);
    mBird.setTexture(context.mTextures->get(Textures::Bird));
    mBird.setNumFrames(8);
    mBird.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mBird.setDuration(sf::seconds(0.7f));
    mBird.centerBottom();
    mBird.setRepeating(true);
    mBird.setPosition(330.f, 900.f - 80.f - 250.f);
}

bool MenuState::handleEvent(User user)
{
    if (!mDrawPopup)
        for (int i = 0; i < 4; i++)
            mButtons[i]->handleEvent(user);
    if (mDrawPopup > 0)
        mButtons[4]->handleEvent(user);
    if (mDrawPopup == 2)
        mButtons[5]->handleEvent(user);
    if (mDrawPopup == 3)
    {
        mButtons[6]->handleEvent(user);
        mButtons[7]->handleEvent(user);
    }
    if (mDrawPopup == 1)
    {
        if (user.isMouseWheelUp && mView.getCenter().y - 0.5f * mView.getSize().y > 0.f)
        {
            mView.move(0.f, -50.f);
            mScrollRect.move(0.f, -50.f * 0.75f * 900.f / 1400.f);
        }
        if (user.isMouseWheelDown && mView.getCenter().y + 0.5f * mView.getSize().y < 1400.f)
        {
            mView.move(0.f, 50.f);
            mScrollRect.move(0.f, 50.f * 0.75f * 900.f / 1400.f);
        }
    }

    return false;
}

bool MenuState::update(sf::Time dt)
{
    mGirl.update(dt);
    mBird.update(dt);
    mBirdGreen.update(dt);
    if (mDrawPopup == 3)
    {
        mButtons[4]->update(dt);
        mButtons[6]->update(dt);
        mButtons[7]->update(dt);
        if (mButtons[4]->isPressed() && mButtonPressed[4] == false)
        {
            mButtonPressed[4] = true;
            mDrawPopup = false;
            mButtons[4]->reset();
        }
        else if (mButtons[6]->isPressed())
        {
            mMusic.stop();
            requestStackPop();
            requestStackPush(States::Countdown);
        }
        else if (mButtons[7]->isPressed())
        {
            mMusic.stop();
            requestStateClear();
            requestStackPush(States::Begin);
            requestStackPush(States::Instruct);
        }
    }
    else if (!mDrawPopup)
    {
        for (int i = 0; i < 4; i++)
            mButtons[i]->update(dt);
        if (mButtons[0]->isPressed() && mButtonPressed[0] == false)
        {
            if (getStackSize() > 1)
            {
                mDrawPopup = 3;
                mButtonPressed[0] = true;
                mButtons[0]->reset();

                mPopup.setPosition(0.5f * 1600.f - 0.5f * mPopup.getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopup.getLocalBounds().height);
                mButtons[4]->setPosition(0.5f * 1600.f + 0.5f * mPopup.getLocalBounds().width - 0.5f * mButtons[4]->getLocalBounds().width - 25.f, 0.5f * 900.f - 0.5f * mPopup.getLocalBounds().height + 0.5f * mButtons[4]->getLocalBounds().height + 25.f);
                mPopupText.clear();
                mPopupText.push_back(sf::Text("Notification", getContext().mFonts->get(Fonts::Title), 70));
                mPopupText.back().setColor(sf::Color(63, 105, 195));
                mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 110.f + 81.5f + 20.f);
                mPopupText.push_back(sf::Text("Continue your last game?", getContext().mFonts->get(Fonts::Main), 70));
                mPopupText.back().setColor(sf::Color::Black);
                mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 375.f);
            }
            else
            {
                mMusic.stop();
                requestStackPop();
                requestStackPush(States::Begin);
                requestStackPush(States::Instruct);
            }
        }

        else if (mButtons[1]->isPressed() && mButtonPressed[1] == false)
        {
            mDrawPopup = 1;
            mButtonPressed[1] = true;
            mButtons[1]->reset();

            mPopupRect.setPosition(0.5f * 1600.f - 0.5f * mPopupRect.getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height);
            mView.reset(sf::FloatRect(0.f, 0.f, mPopupRect.getLocalBounds().width, mPopupRect.getLocalBounds().height - 130.f));
            mView.setViewport(sf::FloatRect(mPopupRect.getPosition().x / 1600.f, (mPopupRect.getPosition().y + 125.f) / 900.f, mPopupRect.getLocalBounds().width / 1600.f, (mPopupRect.getLocalBounds().height - 140.f) / 900.f));

            mScrollRect.setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getLocalBounds().width - 0.5f * mScrollRect.getSize().x - 30.f, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height + 125.f);

            mButtons[4]->setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getLocalBounds().width - 0.5f * mButtons[4]->getLocalBounds().width - 25.f, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height + 0.5f * mButtons[4]->getLocalBounds().height + 25.f);

            mPopupText.clear();
            mPopupText.push_back(sf::Text("About", getContext().mFonts->get(Fonts::Title), 70));
            mPopupText.back().setColor(sf::Color(63, 105, 195));
            mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 110.f + 20.f);
            std::ifstream file("resources/texts/about.data");
            std::string line;
            while (std::getline(file, line))
            {
                mPopupText.push_back(sf::Text(line, getContext().mFonts->get(Fonts::Main), 50));
                mPopupText.back().setColor(sf::Color::Black);
                mPopupText.back().setOrigin(mPopupText.back().getLocalBounds().left, 0.f);
                mPopupText.back().setPosition(100.f, mPopupText.back().getLocalBounds().height + 60.f * (mPopupText.size() - 2));
            }
            file.close();
        }

        else if (mButtons[2]->isPressed() && mButtonPressed[2] == false)
        {
            mDrawPopup = 2;
            mButtonPressed[2] = true;
            mButtons[2]->reset();
            mPopupRect.setPosition(0.5f * 1600.f - 0.5f * mPopupRect.getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height);
            mButtons[4]->setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getLocalBounds().width - 0.5f * mButtons[4]->getLocalBounds().width - 25.f, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height + 0.5f * mButtons[4]->getLocalBounds().height + 25.f);

            mPopupText.clear();
            mPopupText.push_back(sf::Text("Highscore", getContext().mFonts->get(Fonts::Title), 70));
            mPopupText.back().setColor(sf::Color(63, 105, 195));
            mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 110.f + 20.f);
            std::ifstream file("resources/texts/highscore.data");
            std::string line;
            while (std::getline(file, line))
            {
                mPopupText.push_back(sf::Text(line, getContext().mFonts->get(Fonts::Main), 80));
                mPopupText.back().setColor(sf::Color::Black);

                // origin to the right top of mPopupText
                mPopupText.back().setOrigin(mPopupText.back().getLocalBounds().left + mPopupText.back().getLocalBounds().width, 0.f);
                mPopupText.back().setPosition(1000.f, 210.f + mPopupText.back().getLocalBounds().height + 115.f * (mPopupText.size() - 2));
            }
            file.close();
        }

        else if (mButtons[3]->isPressed() && mButtonPressed[3] == false)
        {
            mButtonPressed[3] = true;
            setPlayMusic();
            if (playMusic())
            {
                mMusic.setVolume(70);
                mButtons[3]->setTexture(getContext().mTextures->get(Textures::ButtonMusic));
            }
            else
            {
                mMusic.setVolume(0);
                mButtons[3]->setTexture(getContext().mTextures->get(Textures::ButtonMute));
            }
        }
    }
    else
    {
        mButtons[4]->update(dt);
        if (mDrawPopup == 2)
            mButtons[5]->update(dt);
        if (mDrawPopup == 2 && mButtons[5]->isPressed() && mButtonPressed[5] == false)
        {
            mButtonPressed[5] = true;
            std::ofstream out("resources/texts/highscore.data");
            out << "0\n0\n0";
            out.close();
            mPopupText.clear();
            mPopupText.push_back(sf::Text("Highscore", getContext().mFonts->get(Fonts::Title), 70));
            mPopupText.back().setColor(sf::Color(63, 105, 195));
            mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 110.f + 20.f);
            std::ifstream file("resources/texts/highscore.data");
            std::string line;
            while (std::getline(file, line))
            {
                mPopupText.push_back(sf::Text(line, getContext().mFonts->get(Fonts::Main), 80));
                mPopupText.back().setColor(sf::Color::Black);
                mPopupText.back().setOrigin(mPopupText.back().getLocalBounds().left + mPopupText.back().getLocalBounds().width, 0.f);
                mPopupText.back().setPosition(1000.f, 210.f + mPopupText.back().getLocalBounds().height + 115.f * (mPopupText.size() - 2));
            }
            file.close();
        }
        if (mButtons[4]->isPressed() && mButtonPressed[4] == false)
        {
            mButtonPressed[4] = true;
            mDrawPopup = false;
            mButtons[4]->reset();
        }
    }

    for (int i = 0; i < 6; i++)
        if (!mButtons[i]->isPressed())
            mButtonPressed[i] = false;

    return false;
}

void MenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.setView(target.getDefaultView());
    target.draw(mBackgroundSprite, states);
    target.draw(mGroundSprite, states);
    target.draw(mTitle, states);
    target.draw(mTitleSprite, states);
    target.draw(mGirl, states);
    target.draw(mBird, states);
    target.draw(mBirdGreen, states);

    for (int i = 0; i < 4; i++)
        target.draw(*mButtons[i], states);

    if (mDrawPopup == 1)
    {
        target.draw(mBackgroundRect, states);
        target.draw(mPopupRect, states);
        target.draw(*mButtons[4], states);
        target.draw(mScrollRect, states);
        target.draw(mPopupText[0], states);
        target.setView(mView);
        for (int i = 1; i < mPopupText.size(); i++)
            target.draw(mPopupText[i], states);
        target.setView(target.getDefaultView());
    }
    else if (mDrawPopup == 2)
    {
        target.draw(mBackgroundRect, states);
        target.draw(mPopupRect, states);
        target.draw(mRank, states);
        target.draw(*mButtons[4], states);
        target.draw(*mButtons[5], states);
        for (auto &text : mPopupText)
            target.draw(text, states);
    }
    else if (mDrawPopup == 3)
    {
        target.draw(mBackgroundRect, states);
        target.draw(mPopup, states);
        target.draw(*mButtons[4], states);
        target.draw(*mButtons[6], states);
        target.draw(*mButtons[7], states);
        for (auto &text : mPopupText)
            target.draw(text, states);
    }
}
