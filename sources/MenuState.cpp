#include <MINE/MenuState.hpp>
#include <iostream>

MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context), mBackgroundSprite(), mButtons()
{
    sf::Texture &texture = context.mTextures->get(Textures::PinkBackground);
    sf::Font &font = context.mFonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.mWindow->getSize());

    mBackgroundSprite.setTexture(texture);
    mBackgroundSprite.setPosition(0.f, 0.f);

    mButtons[0] = std::make_shared<Button>(context);
    mButtons[0]->setText("Play", 70);
    mButtons[0]->setPosition(0.5f * windowSize.x, 0.5f * windowSize.y);

    mButtons[1] = std::make_shared<Button>(context);
    mButtons[1]->setText("Back", 70);
    mButtons[1]->setPosition(0.5f * windowSize.x, 0.75f * windowSize.y);
}

bool MenuState::handleEvent(User user)
{
    for (auto &button : mButtons)
        button->handleEvent(user);

    return true;
}

bool MenuState::update(sf::Time dt)
{
    for (auto &button : mButtons)
        button->update(dt);

    if (mButtons[0]->isPressed())
    {
        requestStackPop();
        requestStackPush(States::Playing);
    }
    else if (mButtons[1]->isPressed())
    {
        requestStackPop();
        requestStackPush(States::Intro);
    }
    return true;
}

void MenuState::render()
{
    sf::RenderWindow &mWindow = *getContext().mWindow;
    mWindow.draw(mBackgroundSprite);

    for (auto &button : mButtons)
        mWindow.draw(*button);
}
