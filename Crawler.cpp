#include "Crawler.h"
#include "Game.h"

#include <iostream>

const float Crawler::mSpeed = 60;

Crawler::Crawler(float x, float y)
	: Entity(x, y)
    , mDirection(-1)
    , mSpeedScale(1)  // default speed
    , mIdleRenderable(NULL)
    , mWalkRenderable(NULL)
	, mDieRenderable(NULL)
    , mState(CRAWLER_INIT)
	, mCollisionRect(0,0,0,0)
    , mNextThinkTime(0)
{
    //
    // initialize animation states
    //

    GG::TextureManager* texMgr = Game::GetInstance()->GetTextureManager();

    GG::Texture* idleTex = texMgr->GetTexture("CrawlerIdle");
    mIdleRenderable = new GG::Renderable(idleTex, 0.5f, true);

    GG::Texture* walkTex = texMgr->GetTexture("CrawlerWalk");
    mWalkRenderable = new GG::Renderable(walkTex, 0.5f, true);

	GG::Texture* dieTex = texMgr->GetTexture("CrawlerDie");
    mDieRenderable = new GG::Renderable(dieTex, 0.5f, false);

	// set time to live to duration of jumping animation
    mTimeToDeath = mDieRenderable->GetDuration();

    //
    // initialize AI
    //

    if (GG::UnitRandom() < 0.5f)
	{
        SetState(CRAWLER_IDLE);
    } else
	{
        SetState(CRAWLER_WALK);
    }
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
	mCollisionRect.w = 2*mRect.w/3;
	mCollisionRect.y = mRect.y + 3*mRect.h / 4;
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

	case CRAWLER_DEAD:
	{
		break;
	}

    default:
        // shouldn't happen
        return;
    }

    mState = newState;
}

void Crawler::Update(float dt)
{
    Game* game = Game::GetInstance();

    switch (mState)
	{
    case CRAWLER_WALK:
    {
        if (game->GetTime() >= mNextThinkTime)
		{
            SetState(CRAWLER_IDLE);
        }
		else
		{
            // advance animation
            mRenderable->Animate(dt * mSpeedScale);

            // move position 
            mPosX += dt * mSpeed * mSpeedScale * mDirection;

            // deal with screen edge collisions
            int scrWidth = Game::GetInstance()->GetScrWidth();
            if (GetLeft() < 0)
			{
                SetLeft(0);
                Reverse();
            } 
			else if (GetRight() >= scrWidth)
			{
				SetRight((float)(scrWidth - 1));
                Reverse();
            }

            // update the on-screen rect position (x is the only coordinate that changes)
            mRect.x = (int)GetLeft();
			mCollisionRect.x = mRect.x + mRect.w / 5;
        }
        break;
    }
    
    case CRAWLER_IDLE:
    {
        if (game->GetTime() >= mNextThinkTime)
		{
            SetState(CRAWLER_WALK);

        } 
		else
		{
            // just advance the animation (at regular speed)
            mRenderable->Animate(dt);
        }
        break;
    }

	case CRAWLER_DYING:
    {
        // just advance the animation (at regular speed)
        mRenderable->Animate(dt);
		mTimeToDeath -= dt;
		if (mTimeToDeath <= 0)
		{
			SetState(CRAWLER_DEAD);
		}
        break;
    }

    default:
        // shouldn't happen
        break;
    }
}