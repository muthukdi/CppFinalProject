#include "Robot.h"
#include "Game.h"

Robot::Robot(int x, int y)
	: mRenderableIdle(NULL), mRenderableRun(NULL), mRenderableJump(NULL), 
	mRect(0, 0, 0, 0), mDirection(1), mMotion(0), mJumping(0),
	mOrigX(x), mOrigY(y)
{
	Game* game = Game::GetInstance();
	GG::TextureManager* texMgr = game->GetTextureManager();
	GG::Texture *tex = texMgr->GetTexture("RobotIdle");
	mRenderableIdle = new GG::Renderable(tex, 1.0f, true);
	tex = texMgr->GetTexture("RobotRun");
	mRenderableRun = new GG::Renderable(tex, 0.5f, true);
	tex = texMgr->GetTexture("RobotJump");
	mRenderableJump = new GG::Renderable(tex, 1.0f, true);
	mRect.x = x;
	mRect.y = y;
	mRect.w = mRenderableIdle->GetWidth();
	mRect.h = mRenderableIdle->GetHeight();
	// set time to live to duration of jumping animation
    mTimeToLiveForJump = mRenderableJump->GetDuration();
}

Robot::~Robot()
{
	delete mRenderableIdle;
	delete mRenderableRun;
	delete mRenderableJump;
}

void Robot::Update(float dt)
{
	const float runningSpeed = 200;  // in pixels per second
	const float jumpingSpeed = 400;  // in pixels per second
	Game* game = Game::GetInstance();
	
	if (game->IsKeyDown(SDL_SCANCODE_SPACE) || mJumping == 1)
	{
		if (mJumping == 0)
		{
			mJumping = 1;
			// Raise it a little bit off the ground
			mRect.y -= dt;
		}
		mRenderableJump->Animate(dt);
		mTimeToLiveForJump -= dt;
		if (mTimeToLiveForJump > mRenderableJump->GetDuration()/2.0f)
		{
			// Going up
			mRect.y -= dt * jumpingSpeed;
		}
		else
		{
			// Going down
			mRect.y += dt * jumpingSpeed;
		}
		if (IsFinishedJumping() && mRect.y >= mOrigY)
		{
			mJumping = 0;
			mRenderableJump->Rewind();
			mTimeToLiveForJump = mRenderableJump->GetDuration();
			mRect.y = mOrigY;
		}
	}
	else if (game->IsKeyDown(SDL_SCANCODE_A) || game->IsKeyDown(SDL_SCANCODE_D))
	{
		// If the robot is starting to run from being idle
		if (mMotion == 0)
		{
			mMotion = 1;
			mRenderableRun->Rewind();
		}
		mRenderableRun->Animate(dt);
	}
	else
	{
		// If the robot stops now after running
		if (mMotion = 1)
		{
			mMotion = 0;
		}
		mRenderableIdle->Animate(dt);
	}

    if (game->IsKeyDown(SDL_SCANCODE_A)) 
	{
		if (mDirection == 0)
		{
			mDirection = 1;
		}
		if (mRect.x <= -10.0)
		{
			mRect.x = -10.0;
		}
		else
		{
			mRect.x -= ceil(dt * runningSpeed);
		}
    }
    if (game->IsKeyDown(SDL_SCANCODE_D))
	{
		if (mDirection == 1)
		{
			mDirection = 0;
		}
		if (mRect.x >= game->GetScrWidth() + 64.0 - mRect.w)
		{
			mRect.x = -10.0;
		}
		else
		{
			mRect.x += ceil(dt * runningSpeed);
		}
    }
}