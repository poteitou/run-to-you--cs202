#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <MINE/Animation.hpp>

Animation::Animation()
: mSprite()
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
{
}

Animation::Animation(const sf::Texture& texture)
: mSprite(texture)
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
{
}

void Animation::centerOrigin()
{
	sf::FloatRect bounds = getLocalBounds();
	mSprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void Animation::centerBottom()
{
	sf::FloatRect bounds = getLocalBounds();
	mSprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height));
}

void Animation::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
}

void Animation::setPosition(const sf::Vector2f& position)
{
	mSprite.setPosition(position);
}

void Animation::setTexture(const sf::Texture& texture)
{
	mSprite.setTexture(texture);
}

const sf::Texture* Animation::getTexture() const
{
	return mSprite.getTexture();
}

const sf::Sprite& Animation::getSprite() const
{
	return mSprite;
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	mFrameSize = frameSize;
	mSprite.setTextureRect(sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y));
}

sf::Vector2i Animation::getFrameSize() const
{
	return mFrameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return mNumFrames;
}

void Animation::setDuration(sf::Time duration)
{
	mDuration = duration;
}

sf::Time Animation::getDuration() const
{
	return mDuration;
}

void Animation::setRepeating(bool flag)
{
	mRepeat = flag;
}

bool Animation::isRepeating() const
{
	return mRepeat;
}

void Animation::restart()
{
	mCurrentFrame = 0;
}

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

bool Animation::isNotLastFrame() const
{
	return mCurrentFrame < mNumFrames - 1;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return mSprite.getGlobalBounds();
}

sf::Vector2f Animation::getPosition() const
{
	return mSprite.getPosition();
}

void Animation::move(float x, float y)
{
	mSprite.move(x, y);
}

void Animation::move(const sf::Vector2f &offset)
{
	mSprite.move(offset);
}

void Animation::update(sf::Time dt)
{
	sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
	mElapsedTime += dt;

	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	sf::IntRect textureRect = mSprite.getTextureRect();

	if (mCurrentFrame >= mNumFrames)
		mCurrentFrame = 0;
	if (mCurrentFrame == 0)
		textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
	
	// While we have a frame to process
	while (mElapsedTime >= timePerFrame && (mCurrentFrame < mNumFrames || mRepeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			// Move it down one line
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		// And progress to next frame
		mElapsedTime -= timePerFrame;
		if (mRepeat)
		{
			mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;

			if (mCurrentFrame == 0)
				textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
		}
		else
		{
			mCurrentFrame++;
		}
	}

	mSprite.setTextureRect(textureRect);
}

// set Animation stop at given frame
void Animation::setFrame(std::size_t frame)
{
	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	sf::IntRect textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
	mCurrentFrame = 0;

	while (mCurrentFrame < frame)
	{
		textureRect.left += textureRect.width;

		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		mCurrentFrame++;
	}
	mSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}
