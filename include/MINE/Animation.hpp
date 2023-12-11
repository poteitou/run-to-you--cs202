#ifndef MINE_ANIMATION_HPP
#define MINE_ANIMATION_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <cmath>

class Animation : public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	explicit Animation(const sf::Texture &texture);

	void centerOrigin();
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f &position);

	void setTexture(const sf::Texture &texture);
	const sf::Texture *getTexture() const;
	const sf::Sprite &getSprite() const;

	void setFrameSize(sf::Vector2i mFrameSize);
	sf::Vector2i getFrameSize() const;

	void setNumFrames(std::size_t numFrames);
	std::size_t getNumFrames() const;

	void setDuration(sf::Time duration);
	sf::Time getDuration() const;

	void setRepeating(bool flag);
	bool isRepeating() const;

	void restart();
	bool isFinished() const;
	bool isNotLastFrame() const;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f getPosition() const;

	void move(float x, float y);
	void move(const sf::Vector2f &offset);
	void update(sf::Time dt);
	void setFrame(std::size_t frame);

private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;
	std::size_t mNumFrames;
	std::size_t mCurrentFrame;
	sf::Time mDuration;
	sf::Time mElapsedTime;
	bool mRepeat;
};

#endif // MINE_ANIMATION_HPP
