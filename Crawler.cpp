#include "Crawler.h"
#include "Game.h"

#include <iostream>

const float Crawler::mSpeed = 60;

Crawler::Crawler(float x, float y, bool jumpedOn)
: mRenderable(NULL)
, mRect()
, mPosX(x)
, mPosY(y)
, mDirection(-1)
, mSpeedScale(1)  // default speed
, mIdleRenderable(NULL)
, mWalkRenderable(NULL)
, mDieRenderable(NULL)
, mState(CRAWLER_INIT)
, mCollisionRect(0, 0, 0, 0)
, mNextThinkTime(0)
, mJumpedOn(jumpedOn)
{
}

Crawler::~Crawler()
{
	delete mIdleRenderable;
	delete mWalkRenderable;
	delete mDieRenderable;
}

void Crawler::Reverse()
{
	mDirection = -mDirection;
}

void Crawler::SetDirection(int dir)
{
	if (dir == -1 && mDirection != -1)
	{
		Reverse();
	}
	else if (dir == 1 && mDirection != 1) {
		Reverse();
	}
}

void Crawler::SetRenderable(GG::Renderable* renderable)
{
	mRenderable = renderable;

	if (mRenderable)
	{
		// make the size of the on-screen rectangle match the size of the renderable
		mRect.w = mRenderable->GetWidth();
		mRect.h = mRenderable->GetHeight();

		// set top-left corner of screen rect based on position coordinates
		mRect.x = (int)(mPosX);
		mRect.y = (int)(mPosY - mRect.h);         // y-coord of position is at the bottom of screen rect

	}
	else
	{
		// no renderable
		mRect.w = 0;
		mRect.h = 0;
	}

	mCollisionRect.x = mRect.x + mRect.w / 5;
	mCollisionRect.w = 2 * mRect.w / 3;
	mCollisionRect.y = mRect.y + 3 * mRect.h / 4;
	mCollisionRect.h = mRect.h / 4;
}

void Crawler::SetState(AIState newState)
{
	if (newState == mState) {
		// we're already in this state already
		return;
	}

	Game* game = Game::GetInstance();

	switch (newState) {
	case CRAWLER_IDLE:
	{

		mIdleRenderable->Rewind();
		SetRenderable(mIdleRenderable);

		int numCycles = GG::RandomIntInclusive(2, 5);
		mNextThinkTime = game->GetTime() + numCycles * mIdleRenderable->GetDuration();

		break;
	}

	case CRAWLER_WALK:
	{
		mWalkRenderable->Rewind();
		SetRenderable(mWalkRenderable);

		SetSpeedScale(GG::RandomFloat(0.5f, 2.0f));

		int numCycles = GG::RandomIntInclusive(5, 10);
		mNextThinkTime = game->GetTime() + numCycles * mWalkRenderable->GetDuration();

		break;
	}

	case CRAWLER_DYING:
	{

		mIdleRenderable->Rewind();
		SetRenderable(mDieRenderable);

		break;
	}

	default:
		// shouldn't happen
		return;
	}

	mState = newState;
}
