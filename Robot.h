#ifndef ROBOT_H_
#define ROBOT_H_

#include "GG_Renderable.h"

class Robot 
{
	GG::Renderable*			mRenderableIdle;
	GG::Renderable*			mRenderableRun;
	GG::Renderable*			mRenderableJump;
	GG::Renderable*			mRenderableDie;
	GG::Rect				mRect;
	GG::Rect				mCollisionRect;
	bool					mDirection;  // (1 for left and 0 for right)
	bool					mMotion; // (1 for running and 0 for idle)
	bool					mJumping; // (1 for jumping and 0 for not jumping)
	bool					mDead; // (1 for dead and 0 for not dead)
	int						mOrigX;
	int						mOrigY;
	float					mVelocityY;
	static const int		GRAVITY;

public:
							Robot(int x, int y);
							~Robot();
	const GG::Renderable*	GetRenderableIdle() const			{ return mRenderableIdle;}
	const GG::Renderable*	GetRenderableRun() const			{ return mRenderableRun;}
	const GG::Renderable*	GetRenderableJump() const			{ return mRenderableJump;}
	const GG::Renderable*	GetRenderableDie() const			{ return mRenderableDie;}
	const GG::Rect&			GetRect() const						{ return mRect; }
	const GG::Rect&			GetCollisonRect() const				{ return mCollisionRect; }

	const bool				GetDirection() const				{ return mDirection; }
	void					SetDirection(bool d)				{ mDirection = d; }
	const bool				GetJumping() const					{ return mJumping; }
	const bool				IsDead() const						{ return mDead; }
	void					KillRobot()							{ mDead = 1; }
	void					Bounce(float velocity, bool killed);
	const float				GetVerticalVelocity() const			{ return mVelocityY; }
	void					Update(float dt);
};

#endif