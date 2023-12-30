#ifndef MINE_STATE_HPP
#define MINE_STATE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <MINE/Identifier.hpp>
#include <MINE/User.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
	class RenderStates;
	class RenderTarget;
}

class StateStack;
class User;

class State : public sf::Drawable, public sf::Transformable
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(TextureHolder &textures, FontHolder &fonts, SoundBufferHolder &soundbuffers);

		SoundBufferHolder *mSoundBuffers;
		TextureHolder *mTextures;
		FontHolder *mFonts;
	};

public:
	State(StateStack &stack, Context context);
	virtual ~State();

	virtual bool handleEvent(User user) = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();
	void setPlayMusic();
	bool playMusic() const;

	Context getContext() const;

private:
	StateStack *mStack;
	Context mContext;
};

#endif // MINE_STATE_HPP
