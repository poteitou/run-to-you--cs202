#include <Mine/State.hpp>
#include <Mine/StateStack.hpp>

State::Context::Context(TextureHolder &textures, FontHolder &fonts, SoundBufferHolder &soundbuffers)
	: mTextures(&textures), mFonts(&fonts), mSoundBuffers(&soundbuffers)
{
}

State::State(StateStack &stack, Context context)
	: mStack(&stack), mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

void State::setPlayMusic() 
{
	mStack->setPlayMusic();
}

bool State::playMusic() const
{
	return mStack->playMusic();
}

void State::setCount(int count)
{
	mStack->setCount(count);
}

int State::getCount() const
{
	return mStack->getCount();
}

State::Context State::getContext() const
{
	return mContext;
}
