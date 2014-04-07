#include "CrawlerWeak.h"
#include "Game.h"

#include <iostream>


CrawlerWeak::CrawlerWeak(float x, float y, bool jumpedOn)
	: Crawler(x, y, jumpedOn)
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



void CrawlerWeak::Update(float dt)
{
	Game* game = Game::GetInstance();

	// Get the next tile that the crawler is going to walk on
	int tileWidth = game->GetGrid()->TileWidth();
    int tileHeight = game->GetGrid()->TileHeight();
	mTileRect.w = tileWidth;
	mTileRect.h = tileHeight;
	int row = (mCollisionRect.y + mCollisionRect.h + 10)/tileHeight;
	int column;
	if (mDirection == -1)
	{
		column = (mCollisionRect.x + mCollisionRect.w/8)/tileWidth;
	}
	else
	{
		column = (mCollisionRect.x + mCollisionRect.w/2)/tileWidth;
	}
	GG::Renderable *tileRenderable = game->GetGrid()->GetTile(row, column)->GetRenderable();
	mTileRect.x = column*tileWidth;
	mTileRect.y = row*tileHeight;

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


			// deal with edges of the crawler's platform
			if (!tileRenderable)
			{
				// revert to previous position 
				mPosX -= dt * mSpeed * mSpeedScale * mDirection;
				Reverse();
			}
			else
			{
				// move position 
				 mPosX += dt * mSpeed * mSpeedScale * mDirection;
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