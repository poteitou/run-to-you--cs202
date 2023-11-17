// playing state will have background scrolling slower than ground, and clouds scrolling slower than background
#include <MINE/PlayingState.hpp>

PlayingState::PlayingState(StateStack &stack, Context context)
    : State(stack, context), mBackgroundSprite(), mGroundSprite(), mView(context.mWindow->getDefaultView()), mScrollSpeed(100.f), mViewBound(0.f, 0.f, 10000.f, mView.getSize().y)
{
    sf::Texture &backgroundTexture = context.mTextures->get(Textures::PinkBackground);
    sf::Texture &groundTexture = context.mTextures->get(Textures::Ground);
    backgroundTexture.setRepeated(true);
    groundTexture.setRepeated(true);

    sf::IntRect textureRect(0.f, 0.f, 10000.f, mView.getSize().y);

    mBackgroundSprite.setTextureRect(textureRect);
    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setPosition(0.f, 0.f);

    mGroundSprite.setTextureRect(textureRect);
    mGroundSprite.setTexture(groundTexture);
    mGroundSprite.setPosition(0.f, 0.f);


	// float viewHeight = mWorldView.getSize().y;
	// sf::IntRect textureRect(mWorldBounds);
	// textureRect.height += static_cast<int>(viewHeight);

	// // Add the background sprite to the scene
	// std::unique_ptr<SpriteNode> jungleSprite(new SpriteNode(jungleTexture, textureRect));
	// jungleSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
	// mSceneLayers[Background]->attachChild(std::move(jungleSprite));
}

bool PlayingState::handleEvent(User user)
{
    return true;
}

bool PlayingState::update(sf::Time dt)
{
    mView.move(mScrollSpeed * dt.asSeconds(), 0.f);	
    return true;
}

void PlayingState::render()
{
    sf::RenderWindow &mWindow = *getContext().mWindow;
    mWindow.setView(mView);
    mWindow.draw(mBackgroundSprite);
    mWindow.draw(mGroundSprite);
}