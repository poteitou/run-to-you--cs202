#include <MINE/Instruction.hpp>

Instruction::Instruction(StateStack &stack, Context context)
    : State(stack, context), mTime(0.f)
{
    mBackgroundRect.setPosition(0.f, 0.f);
    mBackgroundRect.setFillColor(sf::Color(0, 0, 0, 100));
    mBackgroundRect.setSize(sf::Vector2f(1600.f, 900.f));

    mButtonAnimation[0].setTexture(context.mTextures->get(Textures::ButtonEsc));
    int mWidth = context.mTextures->get(Textures::ButtonEsc).getSize().x;
    int mHeight = context.mTextures->get(Textures::ButtonEsc).getSize().y / 6;
    mButtonAnimation[0].setFrameSize(sf::Vector2i(mWidth, mHeight));
    mButtonAnimation[0].setNumFrames(5);
    mButtonAnimation[0].setDuration(sf::seconds(1.f));
    mButtonAnimation[0].setRepeating(false);

    mWidth = context.mTextures->get(Textures::ButtonUp).getSize().x;
    mHeight = context.mTextures->get(Textures::ButtonUp).getSize().y / 6;
    for (int i = 1; i < 4; i++)
    {
        mButtonAnimation[i].setTexture(context.mTextures->get(Textures::ButtonUp));
        mButtonAnimation[i].setFrameSize(sf::Vector2i(mWidth, mHeight));
        mButtonAnimation[i].setNumFrames(i == 2 ? 3 : 5);
        mButtonAnimation[i].setDuration(i == 2 ? sf::seconds(0.6f) : sf::seconds(1.f));
        mButtonAnimation[i].setRepeating(false);
    }

    mText.push_back(sf::Text("to pause the game", context.mFonts->get(Fonts::Main), 70));
    mText.back().setColor(sf::Color::White);
    mText.back().setPosition(800.f - mText.back().getLocalBounds().width / 2.f + 90.f, 130.f+ 120.f);
    // mButtonAnimation[0] to the left of "to pause the game"
    mButtonAnimation[0].setPosition(mText.back().getPosition().x - mButtonAnimation[0].getLocalBounds().width - 20.f, 130.f+ 120.f);

    mText.push_back(sf::Text("to use single jump", context.mFonts->get(Fonts::Main), 70));
    mText.back().setColor(sf::Color::White);
    mText.back().setPosition(800.f - mText.back().getLocalBounds().width / 2.f - 10.f, 130.f+ 2 * 120.f);
    // mButtonAnimation[1] to the left of "to use single jump"
    mButtonAnimation[1].setPosition(mText.back().getPosition().x - mButtonAnimation[1].getLocalBounds().width - 20.f, 130.f+ 2 * 120.f);

    mText.push_back(sf::Text("to use double jump", context.mFonts->get(Fonts::Main), 70));
    mText.back().setColor(sf::Color::White);
    mText.back().setPosition(800.f - mText.back().getLocalBounds().width / 2.f + 127.f - 10.f, 130.f+ 3 * 120.f);
    // mButtonAnimation[2],[3] to the left of "to use double jump"
    mButtonAnimation[2].setPosition(mText.back().getPosition().x - mButtonAnimation[2].getLocalBounds().width - 20.f - 120.f, 130.f+ 3 * 120.f);
    mButtonAnimation[3].setPosition(mText.back().getPosition().x - mButtonAnimation[3].getLocalBounds().width - 20.f, 130.f+ 3 * 120.f);

    // Button skip
    mButton = std::make_shared<Button>(context, Textures::ButtonSkip);
    mButton->setPosition(0.5f * 1600.f, 0.5f * 900.f - 0.5f * mButton->getLocalBounds().height + 81.5f + 20.f + 81.5f + 81.5f + 81.5f);
}

bool Instruction::handleEvent(User user)
{   
    mButton->handleEvent(user);
    return false;
}

bool Instruction::update(sf::Time dt)
{
    mTime += dt.asSeconds();
    mButton->update(dt);
    if (mButtonAnimation[3].isFinished())
    {
        for (auto &animation : mButtonAnimation)
            animation.restart();
    }
    else if (mButtonAnimation[2].isFinished())
        mButtonAnimation[3].update(dt);
    else if (mButtonAnimation[1].isFinished())
        mButtonAnimation[2].update(dt);
    else if (mButtonAnimation[0].isFinished())
        mButtonAnimation[1].update(dt);
    else 
        mButtonAnimation[0].update(dt);

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
    for (auto &animation : mButtonAnimation)
        target.draw(animation, states);
    target.draw(*mButton, states);
}
