#include <MINE/StateStack.hpp>

#include <cassert>

StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories()
, mPlayMusic(true)
, mCount(1)
{
}

void StateStack::handleEvent(User user)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(user))
			break;
	}

	applyPendingChanges();
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	// Draw all active states from bottom to top
	for (auto &state : mStack)
		target.draw(*state, states);
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

int StateStack::getSize() const
{
	return mStack.size();
}

void StateStack::setPlayMusic()
{
	mPlayMusic = !mPlayMusic;
}

bool StateStack::playMusic() const
{
	return mPlayMusic;
}

void StateStack::setCount(int count)
{
	mCount = count;
}

int StateStack::getCount() const
{
	return mCount;
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
			case Push:
				mStack.push_back(createState(change.stateID));
				break;

			case Pop:
				mStack.pop_back();
				break;

			case Clear:
				mStack.clear();
				break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action)
, stateID(stateID)
{
}
