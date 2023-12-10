#include <Mine/Game.hpp>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: mWindow(sf::VideoMode(1600, 900), "RUN TO YOU", sf::Style::Titlebar | sf::Style::Close)
, mTextures()
, mFonts()
, mSoundBuffers()
, mStateStack(State::Context(mTextures, mFonts, mSoundBuffers))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mIsPaused(false)
{
	mWindow.setMouseCursorVisible(false);
	// mWindow.setKeyRepeatEnabled(false);
	auto desktop = sf::VideoMode::getDesktopMode();
	mWindow.setPosition(sf::Vector2i(desktop.width/2 - mWindow.getSize().x / 2, desktop.height / 2 - mWindow.getSize().y / 2 - 100));

	mTextures.load(Textures::PinkBackground, "resources/textures/PinkBackground.png");
	mTextures.load(Textures::PinkMouse, "resources/textures/PinkMouse.png");
	mTextures.load(Textures::BlueSkirt, "resources/textures/BlueSkirt.png");
	mTextures.load(Textures::Heart, "resources/textures/Heart.png");
	mTextures.load(Textures::Bird, "resources/textures/Bird.png");
	mTextures.load(Textures::Cat, "resources/textures/Cat.png");
	mTextures.load(Textures::Ground, "resources/textures/Ground.png");
	mTextures.load(Textures::Button, "resources/textures/Button.png");
	mSoundBuffers.load(Sounds::Button, "resources/sounds/Button.wav");
	mSoundBuffers.load(Sounds::Heart, "resources/sounds/Heart.wav");
	mSoundBuffers.load(Sounds::Jump, "resources/sounds/Jump.wav");
	mFonts.load(Fonts::Main, "resources/fonts/PixellettersFull.ttf");

mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(50.f, 10.f);
	mStatisticsText.setCharacterSize(50);

	mMouseSprite.setTexture(mTextures.get(Textures::PinkMouse));
	mMouseSprite.setScale(0.2f, 0.2f);

	registerStates();
	mStateStack.pushState(States::Playing);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			if (!mIsPaused)
				update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		if (!mIsPaused)
		{
			updateStatistics(dt);
			render();
		}
	}
}

void Game::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{	
		mIsPaused = false;
		switch (event.type)
        {
        case sf::Event::Closed:
            mWindow.close();
            break;
        case sf::Event::LostFocus:
            mIsPaused = true;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
                mUser.isMousePressed = true;
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left)
                mUser.isMousePressed = false;
            break;
        case sf::Event::MouseMoved:
            mUser.mousePosition = sf::Mouse::getPosition(mWindow);
            break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Return)
				mUser.isEnterPressed = true;
			else if (event.key.code == sf::Keyboard::Space)
				mUser.isSpacePressed = true;
			else if (event.key.code == sf::Keyboard::Escape)
				mUser.isEscapePressed = true;
			else if (event.key.code == sf::Keyboard::Up)
				mUser.isUpPressed = true;
			break;
        case sf::Event::TextEntered:
            if (event.text.unicode == 8) // Backspace
                mUser.keyPress = '@';
            else if ((event.text.unicode >= 48 && event.text.unicode <= 57) || (event.text.unicode >= 97 && event.text.unicode <= 122) || (event.text.unicode >= 65 && event.text.unicode <= 90) || event.text.unicode == 32) // 0 -> 9 or space
                mUser.keyPress = static_cast<char>(event.text.unicode);
            else
                mUser.keyPress = '$';
            break;
        case sf::Event::KeyReleased:
			mUser.isEnterPressed = false;
			mUser.isSpacePressed = false;
			mUser.isEscapePressed = false;
			mUser.keyPress = '$';
            break;
        default:
            break;
        }
		
		if (!mIsPaused)
			mStateStack.handleEvent(mUser);
	}
}

void Game::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Game::render()
{
	mWindow.clear();

	mWindow.draw(mStateStack);

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);
	mWindow.draw(mMouseSprite);

	mWindow.display();
}

void Game::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
	mMouseSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));
}

void Game::registerStates()
{
	mStateStack.registerState<IntroState>(States::Intro);
    mStateStack.registerState<MenuState>(States::Menu);
    // mStateStack.registerState<SettingsState>(States::Setting);
    mStateStack.registerState<PlayingState>(States::Playing);
    mStateStack.registerState<PausedState>(States::Paused);
    // mStateStack.registerState<GameOverState>(States::GameOver);
    // mStateStack.registerState<EndLevelState>(States::EndLevel);
    // mStateStack.registerState<HighScoreState>(States::HighScore);
    // mStateStack.registerState<AboutState>(States::About);
}
