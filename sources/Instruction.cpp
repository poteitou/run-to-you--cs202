#include <MINE/Instruction.hpp>

Instruction::Instruction(StateStack &stack, Context context)
    : State(stack, context)
{
    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
    sf::FloatRect bounds = mTitle.getGlobalBounds();
    mTitle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitle.setColor(sf::Color::White);
    mTitle.setPosition(0.5f * 1600.f, 0.35f * 900.f);

    mEsc.setTexture(context.mTextures->get(Textures::ButtonEsc));
    mEsc.setPosition(0.5f * 1600.f - 0.5f * mEsc.getLocalBounds().width, 0.5f * 900.f - 0.5f * mEsc.getLocalBounds().height);
    int mWidth = context.mTextures->get(Textures::ButtonEsc).getSize().x;
    int mHeight = context.mTextures->get(Textures::ButtonEsc).getSize().y / 6;
    mEsc.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mEsc.setNumFrames(6);
    mEsc.setDuration(sf::seconds(1.2f));
    mEsc.centerBottom();
    mEsc.setRepeating(true);

    mUp.setTexture(context.mTextures->get(Textures::ButtonUp));
    mUp.setPosition(0.5f * 1600.f - 0.5f * mUp.getLocalBounds().width, 0.5f * 900.f - 0.5f * mUp.getLocalBounds().height);
    mWidth = context.mTextures->get(Textures::ButtonUp).getSize().x;
    mHeight = context.mTextures->get(Textures::ButtonUp).getSize().y / 6;
    mUp.setFrameSize(sf::Vector2i(mWidth, mHeight));
    mUp.setNumFrames(6);
    mUp.setDuration(sf::seconds(1.2f));
    mUp.centerBottom();
    mUp.setRepeating(true);

    mText.push_back(sf::Text("Press       to pause the game", context.mFonts->get(Fonts::Main), 70));
    mText.back().setColor(sf::Color::White);
    mText.back().setPosition(800.f - mText.back().getLocalBounds().width / 2.f, 110.f + 81.5f + 20.f);
    mText.push_back(sf::Text("Press       to use single jump", context.mFonts->get(Fonts::Main), 70));
    mText.back().setColor(sf::Color::White);
    mText.back().setPosition(800.f - mText.back().getLocalBounds().width / 2.f, 110.f + 81.5f + 20.f + 81.5f);
    mText.push_back(sf::Text("Press            to use double jump", context.mFonts->get(Fonts::Main), 70));
    mText.back().setColor(sf::Color::White);
    mText.back().setPosition(800.f - mText.back().getLocalBounds().width / 2.f, 110.f + 81.5f + 20.f + 81.5f + 81.5f);
}

bool Instruction::handleEvent(User user)
{   
    mButton->handleEvent(user);
    return false;
}

bool Instruction::update(sf::Time dt)
{
    mEsc.update(dt);
    mUp.update(dt);
    if (mButton->isPressed())
        requestStackPop();
    return false;
}

void Instruction::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBackgroundRect, states);
    for (auto &text : mText)
        target.draw(text, states);
    target.draw(mEsc, states);
    target.draw(mUp, states);
    target.draw(*mButton, states);
}
