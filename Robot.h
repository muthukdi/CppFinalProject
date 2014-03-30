#ifndef ROBOT_H_
#define ROBOT_H_

#include "GG_Renderable.h"

class Robot 
{
	GG::Renderable*			mRenderableIdle;
	GG::Renderable*			mRenderableRun;
	GG::Renderable*			mRenderableJump;
	GG::Rect				mRect;
	GG::Rect				mCollisionRect;
	bool					mDirection;  // (1 for left and 0 for right)
	bool					mMotion; // (1 for running and 0 for idle)
	bool					mJumping; // (1 for jumping and 0 for not jumping)
	int						mOrigX;
	int						mOrigY;
	float					mVelocityY;
	static const int		GRAVITY;

public:
							Robot(int x, int y);
							~Robot();
	const GG::Renderable*	GetRenderableIdle() const		{ return mRenderableIdle;}
	const GG::Renderable*	GetRenderableRun() const		{ return mRenderableRun;}
	const GG::Renderable*	GetRenderableJump() const		{ return mRenderableJump;}
	const GG::Rect&			GetRect() const					{ return mRect; }
	const GG::Rect&			GetCollisonRect() const			{ return mCollisionRect; }

	const bool				GetDirection() const			{ return mDirection; }
	void					SetDirection(bool d)			{ mDirection = d; }
	const bool				GetJumping() const				{ return mJumping; }
	void					Bounce(float velocity);
	const float				GetVerticalVelocity() const		{ return mVelocityY; }
	void					Update(float dt);
};

#endif