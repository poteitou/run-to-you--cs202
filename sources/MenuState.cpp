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

    mGroundSprite.setTexture(context.mTextures->get(Textures::Ground));
    mGroundSprite.setPosition(0.f, 0.f);

    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));

    // small rect when popup will scroll in the right
    mScrollRect.setSize(sf::Vector2f(50.f, 0.1f * 900.f));
    mScrollRect.setFillColor(sf::Color(31, 65, 114));

    mPopupRect.setTexture(context.mTextures->get(Textures::PopupRect));

    mButtons[0] = std::make_shared<Button>(context, Textures::ButtonPlay);
    mButtons[0]->setText("   Play", 100);
    // button on the right botton of the screen
    mButtons[0]->setPosition(1600.f - 50.f - 0.5f * mButtons[0]->getLocalBounds().width , 0.75f * 900.f + 0.5f * mButtons[0]->getLocalBounds().height - 75.f);

    mButtons[1] = std::make_shared<Button>(context, Textures::ButtonAbout);
    // button on the left of Play button[0]
    mButtons[1]->setPosition(1600.f - 50.f - 0.5f * mButtons[1]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 25.f, 0.75f * 900.f + 0.5f * mButtons[1]->getLocalBounds().height);

    mButtons[2] = std::make_shared<Button>(context, Textures::ButtonRank);
    // button on the left of button[1]
    mButtons[2]->setPosition(1600.f - 50.f - 0.5f * mButtons[2]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 200.f, 0.75f * 900.f + 0.5f * mButtons[2]->getLocalBounds().height);

    mButtons[3] = std::make_shared<Button>(context, Textures::ButtonMusic);
    // button on the left of button[2]
    mButtons[3]->setPosition(1600.f - 50.f - 0.5f * mButtons[3]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 375.f, 0.75f * 900.f + 0.5f * mButtons[3]->getLocalBounds().height);

    mCloseButton = std::make_shared<Button>(context, Textures::ButtonX);
    mResetButton = std::make_shared<Button>(context, Textures::Button);
    mResetButton->setText("Reset", 70);
    mResetButton->setPosition(0.5f * 1600.f - 0.5f * mResetButton->getLocalBounds().width, 787.5f - 0.5f * mResetButton->getLocalBounds().height - 25.f);

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
}

bool MenuState::handleEvent(User user)
{
    for (auto &button : mButtons)
        button->handleEvent(user);
    mCloseButton->handleEvent(user);
    mResetButton->handleEvent(user);
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

    return true;
}

bool MenuState::update(sf::Time dt)
{
    mGirl.update(dt);
    if (!mDrawPopup)
    {
        for (auto &button : mButtons)
            button->update(dt);
        if (mButtons[0]->isPressed())
        {
            mMusic.stop();
            requestStackPop();
            if (getStackSize() == 1)
                requestStackPush(States::Begin);
            else requestStackPush(States::Countdown);
        }

        if (mButtons[1]->isPressed() && mButtonPressed[1] == false)
        {
            mDrawPopup = 1;
            mButtonPressed[1] = true;

            mPopupRect.setPosition(0.5f * 1600.f - 0.5f * mPopupRect.getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height);
            // mView equals to mPopupRect.width, mPopupRect.height-125
            mView.reset(sf::FloatRect(0.f, 0.f, mPopupRect.getLocalBounds().width, mPopupRect.getLocalBounds().height - 130.f));
            // mView is a rectangle with the size mPopupRect.width, mPopupRect.height-125 and position mPopupRect.x, mPopupRect.y+125
            mView.setViewport(sf::FloatRect(mPopupRect.getPosition().x / 1600.f, (mPopupRect.getPosition().y + 125.f) / 900.f, mPopupRect.getLocalBounds().width / 1600.f, (mPopupRect.getLocalBounds().height - 140.f) / 900.f));

            // mScrollRect is on the right of mPopupRect
            mScrollRect.setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getLocalBounds().width - 0.5f * mScrollRect.getSize().x - 30.f, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height + 125.f);

            mCloseButton->setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getLocalBounds().width - 0.5f * mCloseButton->getLocalBounds().width - 25.f, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height + 0.5f * mCloseButton->getLocalBounds().height + 25.f);

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

        if (mButtons[2]->isPressed() && mButtonPressed[2] == false)
        {
            mDrawPopup = 2;
            mButtonPressed[2] = true;

            mPopupRect.setPosition(0.5f * 1600.f - 0.5f * mPopupRect.getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height);
            mCloseButton->setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getLocalBounds().width - 0.5f * mCloseButton->getLocalBounds().width - 25.f, 0.5f * 900.f - 0.5f * mPopupRect.getLocalBounds().height + 0.5f * mCloseButton->getLocalBounds().height + 25.f);

            mPopupText.clear();
            mPopupText.push_back(sf::Text("Highscore", getContext().mFonts->get(Fonts::Title), 70));
            mPopupText.back().setColor(sf::Color(63, 105, 195));
            mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 110.f + 20.f);
            std::ifstream file("resources/texts/highscore.data");
            std::string line;
            int cnt = 1;
            while (std::getline(file, line))
            {
                mPopupText.push_back(sf::Text(std::to_string(cnt) + ". " + line, getContext().mFonts->get(Fonts::Main), 50));
                mPopupText.back().setColor(sf::Color::Black);
                                mPopupText.back().setOrigin(mPopupText.back().getLocalBounds().left, 0.f);
                mPopupText.back().setPosition(100.f, mPopupText.back().getLocalBounds().height + 60.f * (mPopupText.size() - 2));
                cnt++;
            }
            file.close();
        }

        if (mButtons[3]->isPressed() && mButtonPressed[3] == false)
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
        mCloseButton->update(dt);
        if (mDrawPopup == 2)
            mResetButton->update(dt);
        if (mDrawPopup == 2 && mResetButton->isPressed() && mButtonPressed[5] == false)
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
            int cnt = 1;
            while (std::getline(file, line))
            {
                mPopupText.push_back(sf::Text(std::to_string(cnt) + ". " + line, getContext().mFonts->get(Fonts::Main), 50));
                mPopupText.back().setColor(sf::Color::Black);
                                mPopupText.back().setOrigin(mPopupText.back().getLocalBounds().left, 0.f);
                mPopupText.back().setPosition(100.f, mPopupText.back().getLocalBounds().height + 60.f * (mPopupText.size() - 2));
                cnt++;
            }
            file.close();
        }
        if (mCloseButton->isPressed() && mButtonPressed[4] == false)
        {
            mButtonPressed[4] = true;
            mDrawPopup = false;
        }
    }

    for (int i = 0; i < 4; i++)
        if (!mButtons[i]->isPressed())
            mButtonPressed[i] = false;
    if (!mCloseButton->isPressed())
        mButtonPressed[4] = false;
    if (!mResetButton->isPressed())
        mButtonPressed[5] = false;

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

    for (auto &button : mButtons)
        target.draw(*button, states);

    if (mDrawPopup == 1)
    {
        target.draw(mBackgroundRect, states);
        target.draw(mPopupRect, states);
        target.draw(*mCloseButton, states);
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
        target.draw(*mCloseButton, states);
        target.draw(*mResetButton, states);
        for (auto &text : mPopupText)
            target.draw(text, states);
        target.setView(target.getDefaultView());
    }
}
