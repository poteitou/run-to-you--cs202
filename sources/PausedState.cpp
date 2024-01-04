#include <MINE/PausedState.hpp>
#include <iostream>

PausedState::PausedState(StateStack &stack, Context context)
    : State(stack, context), mButtons(),
      mTitle("", context.mFonts->get(Fonts::Title), 100)
{
    mTitle.setString("Paused");
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color(170, 226, 255));
    mTitle.setPosition(0.5f * 1600.f, 0.25f * 900.f);

    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
    mButtons[0] = std::make_shared<Button>(context, Textures::Button);
    mButtons[0]->setText("Resume", 70);
    mButtons[0]->setPosition(0.5f * 1600.f, 0.45f * 900.f);

    mButtons[1] = std::make_shared<Button>(context, Textures::Button);
    mButtons[1]->setText("Restart", 70);
    mButtons[1]->setPosition(0.5f * 1600.f, 0.65f * 900.f);

    mButtons[2] = std::make_shared<Button>(context, Textures::Button);
    mButtons[2]->setText("Save & Quit", 70);
    mButtons[2]->setPosition(0.5f * 1600.f, 0.85f * 900.f);
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
    }
    else if (mButtons[1]->isPressed())
    {
        requestStateClear();
        requestStackPush(States::Begin);
    }
    else if (mButtons[2]->isPressed())
    {
        // requestStateClear();
        requestStackPop();
        requestStackPush(States::Menu);
    }
    return false;
}

void PausedState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundRect, states);
    target.draw(mTitle, states);

    for (auto &button : mButtons)
        target.draw(*button, states);
}
