#include <MINE/PausedState.hpp>
#include <iostream>

PausedState::PausedState(StateStack &stack, Context context)
    : State(stack, context), mButtons()
{
    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
    mButtons[0] = std::make_shared<Button>(context);
    mButtons[0]->setText("Resume", 70);
    mButtons[0]->setPosition(0.5f * 1600.f, 0.5f * 900.f);

    mButtons[1] = std::make_shared<Button>(context);
    mButtons[1]->setText("Quit", 70);
    mButtons[1]->setPosition(0.5f * 1600.f, 0.75f * 900.f);
}

bool PausedState::handleEvent(User user)
{
    for (auto &button : mButtons)
        button->handleEvent(user);

    return false;
}

bool PausedState::update(sf::Time dt)
{
    for (auto &button : mButtons)
        button->update(dt);

    if (mButtons[0]->isPressed())
    {
        requestStackPop();
        // requestStackPush(States::Playing);
    }
    else if (mButtons[1]->isPressed())
    {
        requestStateClear();
        requestStackPush(States::Menu);
    }
    return false;
}

void PausedState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundRect, states);

    for (auto &button : mButtons)
        target.draw(*button, states);
}
