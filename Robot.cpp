#include "Robot.h"
#include "Game.h"
#include "Grid.h"

const float Robot::GRAVITY = 2500.0f;

Robot::Robot(float x, float y)
	: mRenderable(NULL)
	, mRect(0,0,0,0)
	, mRenderableIdle(NULL)
	, mRenderableRun(NULL)
	, mRenderableJump(NULL)
	, mRenderableDie(NULL)
	, mRenderableWalk(NULL)
	, mRenderableCelebrate(NULL)
	, mCollisionRect(0,0,0,0)
	, mBottomTileRect(0,0,0,0)
	, mTopTileRect(0,0,0,0)
	, mDirection(0)
	, mJumping(0)
	, mFalling(0)
	, mDead(0)
	, mAutoPilot(0)
	, mLives(5)
	, mJumpDisabled(0)
	, mVelocityY(-850.0f)
{
	Game* game = Game::GetInstance();
	GG::TextureManager* texMgr = game->GetTextureManager();
	GG::Texture *tex = texMgr->GetTexture("RobotIdle");
	mRenderableIdle = new GG::Renderable(tex, 1.0f, true);
	tex = texMgr->GetTexture("RobotRun");
	mRenderableRun = new GG::Renderable(tex, 0.5f, true);
	tex = texMgr->GetTexture("RobotJump");
	mRenderableJump = new GG::Renderable(tex, 1.0f, true);
	tex = texMgr->GetTexture("RobotDie");
	mRenderableDie = new GG::Renderable(tex, 1.4f, false);
	tex = texMgr->GetTexture("RobotWalk");
	mRenderableWalk = new GG::Renderable(tex, 1.0f, true);
	tex = texMgr->GetTexture("RobotCelebrate");
	mRenderableCelebrate = new GG::Renderable(tex, 1.4f, true);

	mRenderable = mRenderableIdle;
	mRect.x = (int)x;
	mRect.y = (int)y;
	mRect.w = (int)mRenderable->GetWidth();
	mRect.h = (int)mRenderable->GetHeight();

	SetCollisionRect();
}

Robot::~Robot()
{
	delete mRenderableIdle;
	delete mRenderableRun;
	delete mRenderableJump;
	delete mRenderableDie;
	delete mRenderableWalk;
	delete mRenderableCelebrate;
}

void Robot::Update(float dt)
{
	const float runningSpeed = 200;  // in pixels per second
	const float walkingSpeed = 10;  // in pixels per second
	Game* game = Game::GetInstance();

	// Get the tiles that're directly above and beneath the robot's feet
	int tileWidth = game->GetGrid()->TileWidth();
    int tileHeight = game->GetGrid()->TileHeight();
	mBottomTileRect.w = mTopTileRect.w = tileWidth;
	mBottomTileRect.h = mTopTileRect.h = tileHeight;
	// Get the bottom tile
	int row = (mCollisionRect.y + mCollisionRect.h)/tileHeight;
	int column = (mCollisionRect.x + mCollisionRect.w/2)/tileWidth;
	GG::Renderable *bottomTileRenderable = game->GetGrid()->GetTile(row, column)->GetRenderable();
	mBottomTileRect.x = column*tileWidth;
	mBottomTileRect.y = row*tileHeight;
	// Get the top tile
	row = (mCollisionRect.y)/tileHeight;
	column = (mCollisionRect.x + mCollisionRect.w/2)/tileWidth;
	GG::Renderable *topTileRenderable = game->GetGrid()->GetTile(row, column)->GetRenderable();
	mTopTileRect.x = column*tileWidth;
	mTopTileRect.y = row*tileHeight;
	//printf("\nRobot(%i, %i, %i, %i)", mCollisionRect.x, mCollisionRect.y, mCollisionRect.w, mCollisionRect.h);

	// This means that the robot's controls have been disabled and the
	// computer is trying to play the game over animation
	if (mAutoPilot)
	{
		if (mDirection)
		{
			mDirection = 0;
		}
		if (mRect.x + mRect.w > game->GetFlagPole()->GetRect().x)
		{
			mRenderable = mRenderableCelebrate;
		}
		else
		{
			mRect.x += (int)ceil(dt * walkingSpeed);
			SetCollisionRect();
		}
		mRenderable->Animate(dt);
		return;
	}

	// Kill the robot with a bounce!  If the robot is dead, it's not allowed to do 
	// anything else until it's brought back to life by using the "R" key (resurrect)
	if (mDead)
	{
		if (mRenderable != mRenderableDie)
		{
			mRenderable = mRenderableDie;
		}
		if (game->IsKeyDown(SDL_SCANCODE_R) && mVelocityY == 0 && mLives > 0)
		{
			mDead = 0;
			mRenderableDie->Rewind();
			mVelocityY = -850.0f;
			mRect.x = 35;
			mRect.y = game->GetScrHeight()-160;
			SetCollisionRect();
			mDirection = 0;
			game->SetScene(0);
			game->LoadScene(game->GetScene(), true);
			return;
		}
		mVelocityY += GRAVITY * dt;
		mRect.y += (int)(dt * mVelocityY);
		// If there is a tile directly beneath the robot's body
		// AND it's on its way down, stop the fall.
		if (bottomTileRenderable && mVelocityY > 0.0f)
		{
			if (mCollisionRect.y + mCollisionRect.h > mBottomTileRect.y)
			{
				mRect.y = mBottomTileRect.y - mRect.h;
				mVelocityY = 0.0f;
			}
		}
		SetCollisionRect();
		
		mRenderable->Animate(dt);
		return;
	}
	// Change movement renderables and states based on key input
	if (mFalling)
	{
		mVelocityY += GRAVITY * dt;
		mRect.y += (int)(dt * mVelocityY);
		// If there is a tile directly beneath the robot's feet
		// AND it's on its way down, stop the fall
		if (bottomTileRenderable && mVelocityY > 0.0f)
		{
			if (mCollisionRect.y + mCollisionRect.h > mBottomTileRect.y)
			{
				mRect.y = mBottomTileRect.y - mRect.h;
				mFalling = 0;
				mVelocityY = -850.0f;
			}
		}
	}
	else if (game->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		if (!mJumpDisabled)
		{
			mJumpDisabled = true;
			if (mJumping == 0 )
			{
				game->PlaySound("Jump");
				mJumping = 1;
				mRenderableJump->Rewind();
				mRenderable = mRenderableJump;
			}
		}
		else if (game->IsKeyDown(SDL_SCANCODE_A) || game->IsKeyDown(SDL_SCANCODE_D))
		{
			//Now we check if he is jumping
			if (mJumping)
			{			
				//If not jumping anymore change the renderable
				if (!mRenderable->IsAnimating())
				{
					mRenderable = mRenderableRun;
				}
			}
			else
			{
				//Else, if the renderable is not Run then set it to run
				if (mRenderable != mRenderableRun)
				{
					mRenderable = mRenderableRun;
				}
			}
		}
		else
		{
			// If the robot stops now after running 
			//Now we check if he is jumpiing
			if (mJumping)
			{
				//If not jumping anymore change the renderable
				if (!mRenderable->IsAnimating())
				{
					mRenderable = mRenderableIdle;
				}
			}
			//Else, if the renderable is not Idle then set it to idle
			else
			{
				if (mRenderable != mRenderableIdle)
				{
					mRenderable = mRenderableIdle;
				}
			}
		}
	}
	else if (game->IsKeyDown(SDL_SCANCODE_A) || game->IsKeyDown(SDL_SCANCODE_D))
	{
		mJumpDisabled = false;
		//Now we check if he is jumping
		if (mJumping)
		{			
			//If not jumping anymore change the renderable
			if (!mRenderable->IsAnimating())
			{
				mRenderable = mRenderableRun;
			}
		}
		else
		{
			//Else, if the renderable is not Run then set it to run
			if (mRenderable != mRenderableRun)
			{
				mRenderable = mRenderableRun;
			}
		}
	}
	else
	{
		mJumpDisabled = false;
		// If the robot stops now after running 
		//Now we check if he is jumpiing
		if (mJumping)
		{
			//If not jumping anymore change the renderable
			if (!mRenderable->IsAnimating())
			{
				mRenderable = mRenderableIdle;
			}
		}
		//Else, if the renderable is not Idle then set it to idle
		else
		{
			if (mRenderable != mRenderableIdle)
			{
				mRenderable = mRenderableIdle;
			}
		}
	}
	//Jumping with gravity
	if (mJumping)
	{
 		mVelocityY += GRAVITY * dt;
		mRect.y += (int)(dt * mVelocityY);
		// If there is a tile directly beneath the robot's feet
		// AND it's on it's way down
		if (bottomTileRenderable && mVelocityY > 0.0f)
		{
			if (mCollisionRect.y + mCollisionRect.h > mBottomTileRect.y)
			{
				mRect.y = mBottomTileRect.y - mRect.h;
				mJumping = 0;
				mVelocityY = -850.0f;
			}
		}
		// If there is a tile directly above the robot's head
		// AND it's on it's way up
		if (topTileRenderable && mVelocityY <= 0.0f)
		{
			if (mCollisionRect.y < mTopTileRect.y + mTopTileRect.h)
			{
				game->StopSounds();
				game->PlaySound("Block");
				mVelocityY = -mVelocityY;
			}
		}
	}
	// Change the robot's horizontal position based on key input
    if (game->IsKeyDown(SDL_SCANCODE_A)) 
	{
		if (!mDirection)
		{
			mDirection = 1;
		}
		if (mRect.x <= -10)
		{
			// If we are still in the first scene,
			// don't let the robot go back
			if (!game->GetScene())
			{
				mRect.x = -10;
			}
			// Else, let the robot go back to the previous
			// scene without any crawlers or coins
			else
			{
				mRect.x = game->GetScrWidth() + 10.0 - mRect.w;
				game->SetScene(game->GetScene() - 1);
				game->LoadScene(game->GetScene(), false);
			}
		}
		else
		{
			mRect.x -= (int)ceil(dt * runningSpeed);
		}
    }
    if (game->IsKeyDown(SDL_SCANCODE_D))
	{
		if (mDirection)
		{
			mDirection = 0;
		}
		if (mRect.x >= game->GetScrWidth() + 10.0 - mRect.w)
		{
			mRect.x = -10;
			game->SetScene(game->GetScene() + 1);
			game->LoadScene(game->GetScene(), true);
		}
		else
		{
			mRect.x += (int)ceil(dt * runningSpeed);
		}
    }

	SetCollisionRect();
	
	// If there is no ground beneath you, then start falling
	// but not if you are already either jumping or falling!
	if (!bottomTileRenderable && !mJumping && !mFalling )
	{
		mFalling = 1;
		mVelocityY = 0.0f;
		mRenderableJump->Rewind();
		mRenderable = mRenderableJump;
	}
	mRenderable->Animate(dt);
}

// Bounce a little after stomping on a crawler or getting killed
void Robot::Bounce(float velocity, bool killed)
{ 
	if (killed)
	{
		mJumping = 0;
		mDead = 1;
	}
	else
	{
		mJumping = 1;
		mRenderable = mRenderableJump;
	}
	mVelocityY = velocity;
}

void Robot::SetCollisionRect()
{
	if (!mDirection)
	{
		mCollisionRect.x = mRect.x + mRect.w / 4 -5;
		mCollisionRect.w = mRect.w / 2;
		mCollisionRect.y = mRect.y + mRect.h / 3 + 5;
		mCollisionRect.h = mRect.h - mRect.h / 3 - 5;
	}
	else
	{
		mCollisionRect.x = mRect.x + mRect.w / 3;
		mCollisionRect.w = mRect.w / 2;
		mCollisionRect.y = mRect.y + mRect.h / 3 + 5;
		mCollisionRect.h = mRect.h - mRect.h / 3 - 5;
	}
}

void Robot::SetAutoPilot(bool mode)	
{
	if (mAutoPilot != mode)
	{
		mAutoPilot = mode; 
		mRenderable = mRenderableWalk;
	}
}