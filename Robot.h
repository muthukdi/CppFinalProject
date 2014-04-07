#ifndef ROBOT_H_
#define ROBOT_H_

#include "GG_Renderable.h"

class Robot{

protected:
	GG::Renderable*         mRenderable;
	GG::Rect				mRect;
	float                   mPosX;
	float                   mPosY;
	GG::Renderable*			mRenderableIdle;
	GG::Renderable*			mRenderableRun;
	GG::Renderable*			mRenderableJump;
	GG::Renderable*			mRenderableDie;

	GG::Rect				mCollisionRect;
	GG::Rect				mTileRect; // the tile that's directly beneath the robot
	bool					mDirection;  // (1 for left and 0 for right)
	bool					mJumping; // (1 for jumping and 0 for not jumping)
	bool					mFalling; // (1 for falling and 0 for not falling)
	bool					mDead; // (1 for dead and 0 for not dead)
	float					mVelocityY;
	static const float		GRAVITY;

public:
							Robot(float x, float y);
							~Robot();
	GG::Renderable*         GetRenderable() const				{ return mRenderable; }
	const GG::Rect&			GetRect() const						{ return mRect; }
	const GG::Rect&			GetCollisonRect() const				{ return mCollisionRect; }
	const GG::Rect&			GetTileRect() const				{ return mTileRect; }

	const bool				GetDirection() const				{ return mDirection; }
	const bool				GetJumping() const					{ return mJumping; }
	const bool				GetFalling() const					{ return mFalling; }
	void					SetCollisionRect();
	const bool				IsDead() const						{ return mDead; }
	void					KillRobot()							{ mDead = 1; }
	void					Bounce(float velocity, bool killed);
	const float				GetVerticalVelocity() const			{ return mVelocityY; }
	void					Update(float dt);
};

#endif