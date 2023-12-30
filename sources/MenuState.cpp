#include <MINE/MenuState.hpp>
#include <iostream>

MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context), 
      mBackgroundSprite(), 
      mButtons(), 
      mTitle("RUN", context.mFonts->get(Fonts::Caro), 200),
      mTitleSprite(context.mTextures->get(Textures::ToYou)),
      mGirl(context.mTextures->get(Textures::BlueSkirt))
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

    mButtons[0] = std::make_shared<Button>(context, Textures::Button);
    mButtons[0]->setText("Play", 70);
    // button on the right botton of the screen
    mButtons[0]->setPosition(1600.f - 100.f - 0.5f * mButtons[0]->getLocalBounds().width , 0.75f * 900.f + 0.5f * mButtons[0]->getLocalBounds().height);

    // mButtons[0]->setPosition(0.5f * 1600.f, 0.5f * 900.f);


    mButtons[1] = std::make_shared<Button>(context, Textures::ButtonAbout);
    // button on the left of Play button[0]
    mButtons[1]->setPosition(1600.f - 100.f - 0.5f * mButtons[1]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 30.f, 0.75f * 900.f + 0.5f * mButtons[1]->getLocalBounds().height + 50.f);

    mButtons[2] = std::make_shared<Button>(context, Textures::ButtonRank);
    // button on the left of button[1]
    mButtons[2]->setPosition(1600.f - 100.f - 0.5f * mButtons[2]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 160.f, 0.75f * 900.f + 0.5f * mButtons[2]->getLocalBounds().height + 50.f);

    mButtons[3] = std::make_shared<Button>(context, Textures::ButtonMusic);
    // button on the left of button[2]
    mButtons[3]->setPosition(1600.f - 100.f - 0.5f * mButtons[3]->getLocalBounds().width - mButtons[0]->getLocalBounds().width - 290.f, 0.75f * 900.f + 0.5f * mButtons[3]->getLocalBounds().height + 50.f);

    for (int i = 0; i < 4; i++)
        mButtonPressed[i] = false;

    if (!mMusic.openFromFile("resources/sounds/FastMusic.ogg"))
		throw std::runtime_error("Music FastMusic could not be loaded.");
    
    if (playMusic())
        mMusic.setVolume(40);
    else
        mMusic.setVolume(0);
    mMusic.setLoop(true);
    mMusic.play();
}

bool MenuState::handleEvent(User user)
{
    for (auto &button : mButtons)
        button->handleEvent(user);

    return true;
}

bool MenuState::update(sf::Time dt)
{
    mGirl.update(dt);
    for (auto &button : mButtons)
        button->update(dt);

    if (mButtons[0]->isPressed())
    {
        mMusic.stop();
        requestStackPop();
        requestStackPush(States::Playing);
    }

    if (mButtons[3]->isPressed() && mButtonPressed[3] == false)
    {
        mButtonPressed[3] = true;
        setPlayMusic();
        if (playMusic())
        {
            mMusic.setVolume(40);
            mButtons[3]->setTexture(getContext().mTextures->get(Textures::ButtonMusic));
        }
        else
        {
            mMusic.setVolume(0);
            mButtons[3]->setTexture(getContext().mTextures->get(Textures::ButtonMute));
        }
    }

    for (int i = 0; i < 4; i++)
        if (!mButtons[i]->isPressed())
            mButtonPressed[i] = false;

    return false;
}

void MenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundSprite, states);
    target.draw(mGroundSprite, states);
    target.draw(mTitle, states);
    target.draw(mTitleSprite, states);
    target.draw(mGirl, states);

    for (auto &button : mButtons)
        target.draw(*button, states);
}
