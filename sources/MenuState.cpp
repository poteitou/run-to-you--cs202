#include <MINE/MenuState.hpp>
#include <iostream>

MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context), 
      mBackgroundSprite(), 
      mButtons(), 
      mTitle("RUN", context.mFonts->get(Fonts::Caro), 200),
      mTitleSprite(context.mTextures->get(Textures::ToYou)),
      mGirl(context.mTextures->get(Textures::BlueSkirt)),
      mDrawPopup(0)
{
    mTitle.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color(63, 105, 195));
    mTitle.setPosition(0.5f * 1600.f - 180.f, 0.25f * 900.f);

    int mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
    int mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 3;
    mGirl.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mGirl.setNumFrames(8);
    mGirl.setDuration(sf::seconds(0.8f));
    mGirl.centerBottom();
    mGirl.setRepeating(true);
    // mGirl is on the left of mTitle
    mGirl.setPosition(0.5f * 1600.f - 180.f - 1.75f * mWidth, 0.25f * 900.f + 0.5f * mHeight);

    mTitleSprite.setPosition(0.5f * 1600.f, 0.25f * 900.f);

    mBackgroundSprite.setTexture(context.mTextures->get(Textures::PinkBackground));
    mBackgroundSprite.setPosition(0.f, 0.f);

    mGroundSprite.setTexture(context.mTextures->get(Textures::Ground));
    mGroundSprite.setPosition(0.f, 0.f);

    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));

    // small rect when popup will scroll in the right
    mScrollRect.setSize(sf::Vector2f(50.f, 0.15f * 900.f));
    mScrollRect.setFillColor(sf::Color(31, 65, 114));

    mButtons[0] = std::make_shared<Button>(context, Textures::Button);
    mButtons[0]->setText("Play", 70);
    // button on the right botton of the screen
    mButtons[0]->setPosition(1600.f - 100.f - 0.5f * mButtons[0]->getLocalBounds().width , 0.75f * 900.f + 0.5f * mButtons[0]->getLocalBounds().height);

    mButtons[1] = std::make_shared<Button>(context, Textures::ButtonAbout);
    // button on the left of Play button[0]
    mButtons[1]->setPosition(1600.f - 100.f - 0.5f * mButtons[1]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 30.f, 0.75f * 900.f + 0.5f * mButtons[1]->getLocalBounds().height + 50.f);

    mButtons[2] = std::make_shared<Button>(context, Textures::ButtonRank);
    // button on the left of button[1]
    mButtons[2]->setPosition(1600.f - 100.f - 0.5f * mButtons[2]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 160.f, 0.75f * 900.f + 0.5f * mButtons[2]->getLocalBounds().height + 50.f);

    mButtons[3] = std::make_shared<Button>(context, Textures::ButtonMusic);
    // button on the left of button[2]
    mButtons[3]->setPosition(1600.f - 100.f - 0.5f * mButtons[3]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 290.f, 0.75f * 900.f + 0.5f * mButtons[3]->getLocalBounds().height + 50.f);

    mCloseButton = std::make_shared<Button>(context, Textures::ButtonX);

    for (int i = 0; i < 5; i++)
        mButtonPressed[i] = false;

    if (!mMusic.openFromFile("resources/sounds/FastMusic.ogg"))
		throw std::runtime_error("Music FastMusic could not be loaded.");
    
    if (playMusic())
        mMusic.setVolume(70);
    else
        mMusic.setVolume(0);
    mMusic.setLoop(true);
    mMusic.play();
}

bool MenuState::handleEvent(User user)
{
    for (auto &button : mButtons)
        button->handleEvent(user);
    mCloseButton->handleEvent(user);
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
        }

        if (mButtons[1]->isPressed() && mButtonPressed[1] == false)
        {
            mDrawPopup = 1;
            mButtonPressed[1] = true;

            mPopupRect.setFillColor(sf::Color(63, 105, 195));
            mPopupRect.setSize(sf::Vector2f(0.75f * 1600.f, 0.75f * 900.f));
            mPopupRect.setOutlineThickness(15.f);
            mPopupRect.setOutlineColor(sf::Color(31, 65, 114));
            mPopupRect.setPosition(0.5f * 1600.f - 0.5f * mPopupRect.getSize().x, 0.5f * 900.f - 0.5f * mPopupRect.getSize().y);
            // mView equals to mPopupRect
            mView.reset(sf::FloatRect(0.f, 0.f, mPopupRect.getSize().x, mPopupRect.getSize().y));
            mView.setViewport(sf::FloatRect(0.5f - 0.5f * mPopupRect.getSize().x / 1600.f, 0.5f - 0.5f * mPopupRect.getSize().y / 900.f, mPopupRect.getSize().x / 1600.f, mPopupRect.getSize().y / 900.f));
            // mScrollRect is on the right of mPopupRect
            mScrollRect.setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getSize().x - 0.5f * mScrollRect.getSize().x - 30.f, 0.5f * 900.f - 0.5f * mPopupRect.getSize().y + 150.f);

            mCloseButton->setPosition(0.5f * 1600.f + 0.5f * mPopupRect.getSize().x - 0.5f * mCloseButton->getLocalBounds().width - 20.f, 0.5f * 900.f - 0.5f * mPopupRect.getSize().y + 0.5f * mCloseButton->getLocalBounds().height + 20.f);

            // mPopupText.push_back(sf::Text("About", getContext().mFonts->get(Fonts::Title), 100));
            // mPopupText.back().setColor(sf::Color::Black);
            // mPopupText.back().setPosition(0.5f * mPopupText.back().getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopupRect.getSize().y);
            std::ifstream file("resources/texts/about.data");
            std::string line;
            mPopupText.clear();
            while (std::getline(file, line))
            {
                mPopupText.push_back(sf::Text(line, getContext().mFonts->get(Fonts::Main), 50));
                mPopupText.back().setColor(sf::Color::White);
                
                // set Origin of text to the left of the text
                mPopupText.back().setOrigin(mPopupText.back().getLocalBounds().left, 0.f);
                mPopupText.back().setPosition(100.f, mPopupText.back().getLocalBounds().height + 60.f * (mPopupText.size() - 1));
            }
            file.close();
        }

        if (mButtons[2]->isPressed() && mButtonPressed[2] == false)
        {
            mDrawPopup = 2;
            mButtonPressed[2] = true;
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
        if (mCloseButton->isPressed() && mButtonPressed[4] == false)
        {
            mButtonPressed[4] = true;
            mDrawPopup = false;
        }
    }

    for (int i = 0; i < 5; i++)
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

    for (auto &button : mButtons)
        target.draw(*button, states);

    if (mDrawPopup)
    {
        target.draw(mBackgroundRect, states);
        target.draw(mPopupRect, states);
        target.draw(*mCloseButton, states);
        target.draw(mScrollRect, states);
        target.setView(mView);
        for (auto &text : mPopupText)
            target.draw(text, states);
        target.setView(target.getDefaultView());
    }
}
