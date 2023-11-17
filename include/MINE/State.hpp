#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <MINE/User.hpp>
#include <MINE/Identifier.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}

class StateStack;
class User;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow &window, TextureHolder &textures, FontHolder &fonts);

		sf::RenderWindow *mWindow;
		TextureHolder *mTextures;
		FontHolder *mFonts;
	};

public:
	State(StateStack &stack, Context context);
	virtual ~State();

	virtual bool handleEvent(User user) = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual void render() = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();

	Context getContext() const;

private:
	StateStack *mStack;
	Context mContext;
};

#endif // STATE_HPP
