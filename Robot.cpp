#include "Robot.h"
#include "Game.h"

const int Robot::GRAVITY = 2500;

Robot::Robot(int x, int y)
	: mRenderableIdle(NULL)
	, mRenderableRun(NULL)
	, mRenderableJump(NULL)
	, mRect(0, 0, 0, 0)
	, mCollisionRect(0,0,0,0)
	, mDirection(1)
	, mMotion(0)
	, mJumping(0)
	, mOrigX(x)
	, mOrigY(y)
	, mVelocityY(-800)
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

	mCollisionRect.x = mRect.x + mRect.w / 3;
	mCollisionRect.w = mRect.w / 2;
	mCollisionRect.y = mRect.y + mRect.h / 3 + 5;
	mCollisionRect.h = mRect.h - mRect.h / 3 - 5;

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
	
	if (game->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		if (mJumping == 0)
		{
			mJumping = 1;
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
	//This is for Gravity
	if (mJumping == 1){		
			mVelocityY += GRAVITY*dt;
			mOrigY += dt * mVelocityY;
			if (mOrigY > game->GetScrHeight()- 160){
				mOrigY = game->GetScrHeight() - 160;
				mJumping = 0;
				mRenderableJump->Rewind();
				mVelocityY = -800;
			}
			mRect.y = mOrigY;
			mRenderableJump->Animate(dt);
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
	if (mDirection == 0){
		mCollisionRect.x = mRect.x + mRect.w / 4 -5;
		mCollisionRect.w = mRect.w / 2;
		mCollisionRect.y = mRect.y + mRect.h / 3 + 5;
		mCollisionRect.h = mRect.h - mRect.h / 3 - 5;

	}
	else{
		mCollisionRect.x = mRect.x + mRect.w / 3;
		mCollisionRect.w = mRect.w / 2;
		mCollisionRect.y = mRect.y + mRect.h / 3 + 5;
		mCollisionRect.h = mRect.h - mRect.h / 3 - 5;
	}
}