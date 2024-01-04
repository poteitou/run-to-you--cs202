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
	// mWindow.setKeyRepeatEnabled(false);
	auto desktop = sf::VideoMode::getDesktopMode();
	mWindow.setPosition(sf::Vector2i(desktop.width/2 - mWindow.getSize().x / 2, desktop.height / 2 - mWindow.getSize().y / 2 - 100));

	mTextures.load(Textures::MenuBG, "resources/textures/MenuBG.png");
	mTextures.load(Textures::PinkBG, "resources/textures/PinkBG.png");
	mTextures.load(Textures::OrangeBG, "resources/textures/OrangeBG.png");
	mTextures.load(Textures::BlueBG, "resources/textures/BlueBG.png");
	mTextures.load(Textures::PinkToOrange, "resources/textures/PinkToOrange.png");
	mTextures.load(Textures::PinkMouse, "resources/textures/PinkMouse.png");
	mTextures.load(Textures::BlueSkirt, "resources/textures/BlueSkirt.png");
	mTextures.load(Textures::CrushRight, "resources/textures/CrushRight.png");
	mTextures.load(Textures::BlueCollide, "resources/textures/BlueCollide.png");
	mTextures.load(Textures::Heart, "resources/textures/Heart.png");
	mTextures.load(Textures::Life, "resources/textures/Life.png");
	mTextures.load(Textures::Bird, "resources/textures/Bird.png");
	mTextures.load(Textures::Dog, "resources/textures/Dog.png");
	mTextures.load(Textures::Tree, "resources/textures/Tree.png");
	mTextures.load(Textures::Rock, "resources/textures/Rock.png");
	mTextures.load(Textures::ToYou, "resources/textures/ToYou.png");
	mTextures.load(Textures::Ground, "resources/textures/Ground.png");
	mTextures.load(Textures::Button, "resources/textures/Button.png");
	mTextures.load(Textures::ButtonMusic, "resources/textures/ButtonMusic.png");
	mTextures.load(Textures::ButtonMute, "resources/textures/ButtonMute.png");
	mTextures.load(Textures::ButtonAbout, "resources/textures/ButtonAbout.png");
	mTextures.load(Textures::ButtonRank, "resources/textures/ButtonRank.png");
	mTextures.load(Textures::ButtonX, "resources/textures/ButtonX.png");
	mSoundBuffers.load(Sounds::Button, "resources/sounds/Button.wav");
	mSoundBuffers.load(Sounds::Paused, "resources/sounds/Paused.wav");
	mSoundBuffers.load(Sounds::GameOver, "resources/sounds/GameOver.wav");
	mSoundBuffers.load(Sounds::Collide, "resources/sounds/Collide.wav");
	mSoundBuffers.load(Sounds::First, "resources/sounds/First.wav");
	mSoundBuffers.load(Sounds::Heart, "resources/sounds/Heart.wav");
	mFonts.load(Fonts::Title, "resources/fonts/Title.ttf");
	mFonts.load(Fonts::Caro, "resources/fonts/Caro.ttf");
	mFonts.load(Fonts::Main, "resources/fonts/Main.ttf");

mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(50.f, 10.f);
	mStatisticsText.setCharacterSize(50);

	if (!mCursorImage.loadFromFile("resources/textures/PinkMouse.png"))
		throw std::runtime_error("Cannot load cursor image");
	sf::Vector2u size = mCursorImage.getSize();
	sf::Uint8* pixels = new sf::Uint8[size.x * size.y * 4];
	memcpy(pixels, mCursorImage.getPixelsPtr(), size.x * size.y * 4);
	mCursor.loadFromPixels(pixels, size, sf::Vector2u(0, 0));
	mWindow.setMouseCursor(mCursor);

	registerStates();
	mStateStack.pushState(States::Intro);
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
		if (event.type == sf::Event::LostFocus)
			mIsPaused = true;
		if (!mIsPaused)
		{
			mUser.isMouseWheelUp = mUser.isMouseWheelDown = false;
			switch (event.type)
			{
			case sf::Event::Closed:
				mWindow.close();
				break;
			case sf::Event::MouseWheelMoved:
				if (event.mouseWheel.delta > 0)
					mUser.isMouseWheelUp = true;
				else if (event.mouseWheel.delta < 0)
					mUser.isMouseWheelDown = true;
				break;
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
					mUser.isSpacePressed = true;
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
	mWindow.draw(mStatisticsText);

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
}

void Game::registerStates()
{
	mStateStack.registerState<IntroState>(States::Intro);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<Countdown>(States::Countdown);
    mStateStack.registerState<BeginState>(States::Begin);
    mStateStack.registerState<PausedState>(States::Paused);
    mStateStack.registerState<GameOverState>(States::GameOver);
	mStateStack.registerState<BTMState>(States::BTM);
    // mStateStack.registerState<EndLevelState>(States::EndLevel);
    // mStateStack.registerState<HighScoreState>(States::HighScore);
}
