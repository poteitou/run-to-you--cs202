#include <MINE/GameOverState.hpp>
#include <iostream>

GameOverState::GameOverState(StateStack &stack, Context context)
    : State(stack, context), mButtons(),
      mTitle("", context.mFonts->get(Fonts::Title), 100)
{
    mTitle.setString("Game Over");
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color(170, 226, 255));
    mTitle.setPosition(0.5f * 1600.f, 0.25f * 900.f);

    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
    mButtons[0] = std::make_shared<Button>(context, Textures::Button);
    mButtons[0]->setText("Replay", 70);
    mButtons[0]->setPosition(0.5f * 1600.f, 0.5f * 900.f);

    mButtons[1] = std::make_shared<Button>(context, Textures::Button);
    mButtons[1]->setText("Quit", 70);
    mButtons[1]->setPosition(0.5f * 1600.f, 0.75f * 900.f);
}

bool GameOverState::handleEvent(User user)
{
    for (auto &button : mButtons)
        button->handleEvent(user);

    return false;
}

bool GameOverState::update(sf::Time dt)
{
    for (auto &button : mButtons)
        button->update(dt);

    if (mButtons[0]->isPressed())
    {
        requestStateClear();
        requestStackPush(States::Playing);
    }
    else if (mButtons[1]->isPressed())
    {
        requestStateClear();
        requestStackPush(States::Menu);
    }
    return false;
}

void GameOverState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundRect, states);
    target.draw(mTitle, states);

    for (auto &button : mButtons)
        target.draw(*button, states);
}
