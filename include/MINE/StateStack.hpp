#ifndef MINE_STATESTACK_HPP
#define MINE_STATESTACK_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <MINE/User.hpp>
#include <MINE/State.hpp>
#include <MINE/Identifier.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf
{
	class Event;
	class RenderWindow;
	class RenderStates;
	class RenderTarget;
}

class StateStack : public sf::Drawable, public sf::Transformable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

public:
	explicit StateStack(State::Context context);

	template <typename T>
	void registerState(States::ID stateID);

	void handleEvent(User user);
	void update(sf::Time dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void pushState(States::ID stateID);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::Ptr createState(States::ID stateID);
	void applyPendingChanges();

private:
	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::None);

		Action action;
		States::ID stateID;
	};

private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;

	State::Context mContext;
	std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext));
	};
}

#endif // MINE_STATESTACK_HPP
