#ifndef CRAWLER_H_
#define CRAWLER_H_

#include "GG_Renderable.h"
#include "GG_Common.h"
#include "Entity.h"

class Crawler : public Entity {

	GG::Rect					mCollisionRect;
    static const float          mSpeed;      // default speed in pixels per second
    int							mDirection;  // (-1 for left and 1 for right)
	float                       mSpeedScale; // speed multiplier (1 is default, <1 is slower, >1 is faster)
	float						mTimeToDeath;    // remaining time left for death animation (in seconds)


    GG::Renderable*             mIdleRenderable;
    GG::Renderable*             mWalkRenderable;
	GG::Renderable*             mDieRenderable;

public:
    enum AIState                { CRAWLER_INIT, CRAWLER_IDLE, CRAWLER_WALK, CRAWLER_DYING, CRAWLER_DEAD };   // possbile AI states

private:
    AIState                     mState;                                         // current AI state

    float                       mNextThinkTime;                                 // time to run next round of AI "thinking"

    void                        SetRenderable(GG::Renderable* renderable);      // private helper method

public:
                                Crawler(float x, float y);
                                ~Crawler();

	const GG::Rect&				GetCollisionRect() const		{ return mCollisionRect; }

    float                       GetLeft() const         { return mPosX; }
    float                       GetRight() const        { return mPosX + mRect.w; }
    float                       GetBottom() const       { return mPosY; }
    float                       GetTop() const          { return mPosY + mRect.h; }
	const int					GetDirection() const	{ return mDirection; }

	const AIState				GetState() const		{ return mState; }

    void                        SetLeft(float x)        { mPosX = x; }
    void                        SetRight(float x)       { mPosX = x - mRect.w; }
    void                        SetBottom(float y)      { mPosY = y; }
    void                        SetTop(float y)         { mPosY = y - mRect.h; }

    void                        Reverse();              // turn around horizontally

    void                        SetDirection(int dir);  // dir should be -1 for left or 1 for right

    void                        SetSpeedScale(float s)  { mSpeedScale = s; }

    void                        Update(float dt);

    void                        SetState(AIState newState);
};

#endif