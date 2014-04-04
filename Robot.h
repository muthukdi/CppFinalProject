#ifndef ROBOT_H_
#define ROBOT_H_

#include "GG_Renderable.h"
#include "Entity.h"

class Robot : public Entity {

protected:
	GG::Renderable*			mRenderableIdle;
	GG::Renderable*			mRenderableRun;
	GG::Renderable*			mRenderableJump;
	GG::Renderable*			mRenderableDie;
	GG::Rect				mCollisionRect;
	bool					mDirection;  // (1 for left and 0 for right)
	bool					mJumping; // (1 for jumping and 0 for not jumping)
	bool					mDead; // (1 for dead and 0 for not dead)
	float					mVelocityY;
	static const float		GRAVITY;

public:
							Robot(float x, float y);
							~Robot();
	const GG::Rect&			GetCollisonRect() const				{ return mCollisionRect; }

	const bool				GetDirection() const				{ return mDirection; }
	const bool				GetJumping() const					{ return mJumping; }
	const bool				IsDead() const						{ return mDead; }
	void					KillRobot()							{ mDead = 1; }
	void					Bounce(float velocity, bool killed);
	const float				GetVerticalVelocity() const			{ return mVelocityY; }
	void					Update(float dt);
};

#endif