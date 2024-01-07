#include <MINE/PausedState.hpp>
#include <iostream>

PausedState::PausedState(StateStack &stack, Context context)
    : State(stack, context), mButtons(),
      mTitle("", context.mFonts->get(Fonts::Title), 110),
      mDrawPopup(false)
{
    mTitle.setString("Paused");
    sf::FloatRect bounds = mTitle.getLocalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color(170, 226, 255));
    mTitle.setPosition(0.5f * 1600.f, 0.35f * 900.f);

    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
    mButtons[0] = std::make_shared<Button>(context, Textures::ButtonResume);
    mButtons[0]->setPosition(0.5f * 1600.f - 190.f, 0.6f * 900.f);

    mButtons[1] = std::make_shared<Button>(context, Textures::ButtonReplay);
    mButtons[1]->setPosition(0.5f * 1600.f, 0.6f * 900.f);

    mButtons[2] = std::make_shared<Button>(context, Textures::ButtonQuit);
    mButtons[2]->setPosition(0.5f * 1600.f + 190.f, 0.6f * 900.f);

    mButtons[3] = std::make_shared<Button>(context, Textures::ButtonX);
    mButtons[4] = std::make_shared<Button>(context, Textures::Button);
    mButtons[4]->setText("OK", 70);
    mButtons[4]->setPosition(0.5f * 1600.f - 0.5 * 450.f, 631.f);
    mButtons[5] = std::make_shared<Button>(context, Textures::Button);
    mButtons[5]->setText("Quit Only", 70);
    mButtons[5]->setPosition(0.5f * 1600.f + 0.5 * 450.f, 631.f);
    mPopup.setTexture(context.mTextures->get(Textures::Popup));

    for (int i = 0; i < 4; i++)
        mButtonPressed[i] = false;

    mPopup.setPosition(0.5f * 1600.f - 0.5f * mPopup.getLocalBounds().width, 0.5f * 900.f - 0.5f * mPopup.getLocalBounds().height);
    mButtons[3]->setPosition(0.5f * 1600.f + 0.5f * mPopup.getLocalBounds().width - 0.5f * mButtons[3]->getLocalBounds().width - 25.f, 0.5f * 900.f - 0.5f * mPopup.getLocalBounds().height + 0.5f * mButtons[3]->getLocalBounds().height + 25.f);
    mPopupText.push_back(sf::Text("Notification", getContext().mFonts->get(Fonts::Title), 70));
    mPopupText.back().setColor(sf::Color(63, 105, 195));
    mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 110.f + 81.5f + 20.f);
    mPopupText.push_back(sf::Text("Save and Quit?", getContext().mFonts->get(Fonts::Main), 70));
    mPopupText.back().setColor(sf::Color::Black);
    mPopupText.back().setPosition(800.f - mPopupText.back().getLocalBounds().width / 2.f, 375.f);
}

bool PausedState::handleEvent(User user)
{
    if (!mDrawPopup)
        for (int i = 0; i < 3; i++)
            mButtons[i]->handleEvent(user);
    else
        for (int i = 3; i < 6; i++)
            mButtons[i]->handleEvent(user);

    return false;
}

bool PausedState::update(sf::Time dt)
{
    if (mDrawPopup)
    {
        for (int i = 3; i < 6; i++)
            mButtons[i]->update(dt);
        if (mButtons[3]->isPressed() && mButtonPressed[3] == false)
        {
            mButtonPressed[3] = true;
            mDrawPopup = false;
            mButtons[3]->reset();
        }
        else if (mButtons[4]->isPressed())
        {
            requestStackPop();
            requestStackPush(States::Menu);
        }
        else if (mButtons[5]->isPressed())
        {
            requestStateClear();
            requestStackPush(States::Menu);
        }
    }
    else if (!mDrawPopup)
    {
        for (int i = 0; i < 3; i++)
            mButtons[i]->update(dt);

        if (mButtons[0]->isPressed())
        {
            requestStackPop();
        }
        else if (mButtons[1]->isPressed())
        {
            requestStateClear();
            requestStackPush(States::Begin);
        }
        else if (mButtons[2]->isPressed() && mButtonPressed[2] == false)
        {
            mButtonPressed[2] = true;
            mDrawPopup = true;
            mButtons[2]->reset();
        }
    }

    for (int i = 0; i < 4; i++)
        if (!mButtons[i]->isPressed())
            mButtonPressed[i] = false;

    return false;
}

void PausedState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundRect, states);
    target.draw(mTitle, states);

    for (int i = 0; i < 3; i++)
        target.draw(*mButtons[i], states);

    if (mDrawPopup)
    {
        // target.draw(mBackgroundRect, states);
        target.draw(mPopup, states);
        for (int i = 3; i < 6; i++)
            target.draw(*mButtons[i], states);
        for (auto &text : mPopupText)
            target.draw(text, states);
    }
}
