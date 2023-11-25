#include <MINE/MenuState.hpp>
#include <iostream>

MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context), mBackgroundSprite(), mButtons()
{
    sf::Texture &texture = context.mTextures->get(Textures::PinkBackground);

    mBackgroundSprite.setTexture(texture);
    mBackgroundSprite.setPosition(0.f, 0.f);

    mButtons[0] = std::make_shared<Button>(context);
    mButtons[0]->setText("Play", 70);
    mButtons[0]->setPosition(0.5f * 1600.f, 0.5f * 900.f);

    mButtons[1] = std::make_shared<Button>(context);
    mButtons[1]->setText("Back", 70);
    mButtons[1]->setPosition(0.5f * 1600.f, 0.75f * 900.f);
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

void MenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundSprite, states);

    for (auto &button : mButtons)
        target.draw(*button, states);
}
