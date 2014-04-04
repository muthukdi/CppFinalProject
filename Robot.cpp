#include "Robot.h"
#include "Game.h"

const float Robot::GRAVITY = 2500.0f;

Robot::Robot(float x, float y)
	: Entity(x, y)
	, mRenderableIdle(NULL)
	, mRenderableRun(NULL)
	, mRenderableJump(NULL)
	, mRenderableDie(NULL)
	, mCollisionRect(0,0,0,0)
	, mDirection(1)
	, mJumping(0)
	, mDead(0)
	, mVelocityY(-800.0f)
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

	mRenderable = mRenderableIdle;
	mRect.x = (int)x;
	mRect.y =(int) y;
	mRect.w = (int)mRenderable->GetWidth();
	mRect.h = (int)mRenderable->GetHeight();

	mCollisionRect.x = mRect.x + mRect.w / 3;
	mCollisionRect.w = mRect.w / 2;
	mCollisionRect.y = mRect.y + mRect.h / 3 + 5;
	mCollisionRect.h = mRect.h - mRect.h / 3 - 5;

}

Robot::~Robot()
{
	delete mRenderableIdle;
	delete mRenderableRun;
	delete mRenderableJump;
	delete mRenderableDie;
}

void Robot::Update(float dt)
{
	const float runningSpeed = 200;  // in pixels per second
	Game* game = Game::GetInstance();
	
	// Kill the robot with a bounce!  If the robot is dead, it's not allowed to do 
	// anything else until it's brought back to life by using the "R" key (resurrect)
	if (mDead == 1)
	{
		if (mRenderable != mRenderableDie){
			mRenderableDie->Rewind();
			mRenderable = mRenderableDie;
		}
		if (game->IsKeyDown(SDL_SCANCODE_R) && mVelocityY == 0)
		{
			mDead = 0;
			mRenderableDie->Rewind();
			mVelocityY = -800;
			mRect.y = game->GetScrHeight() - 160;
			return;
		}
		mVelocityY += GRAVITY * dt;
		mPosY += dt * mVelocityY;
		// If it's below it's original height
		if (mPosY > game->GetScrHeight() - 160)
		{
			mPosY = game->GetScrHeight() - 160.0f;
			mVelocityY = 0.0;
		}
		mRect.y = (int)mPosY;
		mRenderable->Animate(dt);
		return;
	}
	if (game->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		if (mJumping == 0)
		{
			game->PlaySound("Dilip");
			mJumping = 1;
			mRenderableJump->Rewind();
			mRenderable = mRenderableJump;
		}
	}
	else if (game->IsKeyDown(SDL_SCANCODE_A) || game->IsKeyDown(SDL_SCANCODE_D))
	{
		//Now we check if he is jumpiing
		if (mJumping == 1)
		{			
			//If not jumping anymore change the renderable
			if (!mRenderable->IsAnimating())
			{
				mRenderable = mRenderableRun;
			}
		}
		else
		{
			//Else, if the renderable is not Run then set it to idle
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
		if (mJumping == 1)
		{
			//If not jumping anymore change the renderable
			if (!mRenderable->IsAnimating())
			{
				mRenderable = mRenderableIdle;
			}
		}
		//Else, if the renderable is not Idle then set it to idle
		else{
			if (mRenderable != mRenderableIdle)
			{
				mRenderable = mRenderableIdle;
			}
		}
	}
	//Jumping with gravity
	if (mJumping == 1)
	{
 		mVelocityY += GRAVITY * dt;
		mPosY += dt * mVelocityY;
		// If it's below it's original height
		if (mPosY > game->GetScrHeight() - 160.0f)
		{
			mPosY = game->GetScrHeight() - 160.0f;
			mJumping = 0;
			mVelocityY = -800.0f;
		}
 		mRect.y =(int) mPosY;
		
		//mRenderable->Animate(dt);
	}

    if (game->IsKeyDown(SDL_SCANCODE_A)) 
	{
		if (mDirection == 0)
		{
			mDirection = 1;
		}
		if (mRect.x <= -10)
		{
			mRect.x = -10;
		}
		else
		{
			mRect.x -= (int)ceil(dt * runningSpeed);
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
			mRect.x = -10;
		}
		else
		{
			mRect.x += (int)ceil(dt * runningSpeed);
		}
    }
	if (mDirection == 0)
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
mRenderable->Animate(dt);

}

// Bounce a little after stomping on a crawler or getting killed
void Robot::Bounce(float velocity, bool killed)
{ 
	if (killed)
	{
		mDead = 1;
	}
	else
	{
		mJumping = 1;
		mRenderable = mRenderableJump;
	}
	mVelocityY = velocity;
}