#ifndef CRAWLER_H_
#define CRAWLER_H_

#include "GG_Renderable.h"
#include "GG_Common.h"

class Crawler {
	public:
		enum AIState                { CRAWLER_INIT, CRAWLER_IDLE, CRAWLER_WALK, CRAWLER_DYING, CRAWLER_DEAD };   // possbile AI states

	protected:
		GG::Renderable*             mRenderable;
		GG::Rect                    mRect;
		GG::Rect					mCollisionRect;
		GG::Rect					mTileRect; // the tile that's directly beneath the crawler

		float                       mPosX;
		float                       mPosY;

		static const float          mSpeed;      // default speed in pixels per second
		int							mDirection;  // (-1 for left and 1 for right)
		float                       mSpeedScale; // speed multiplier (1 is default, <1 is slower, >1 is faster)
		float						mTimeToDeath;    // remaining time left for death animation (in seconds)


		GG::Renderable*             mIdleRenderable;
		GG::Renderable*             mWalkRenderable;
		GG::Renderable*             mDieRenderable;

		AIState                     mState;                                         // current AI state

		float                       mNextThinkTime;                                 // time to run next round of AI "thinking"

		void                        SetRenderable(GG::Renderable* renderable);      // private helper method

		bool						mJumpedOn;



	public:
		Crawler(float x, float y, bool jumpedOn);
		virtual ~Crawler();

		const GG::Renderable*       GetRenderable() const   { return mRenderable; }
		const GG::Rect&             GetRect() const         { return mRect; }
		const GG::Rect&				GetCollisionRect() const		{ return mCollisionRect; }
		const GG::Rect&				GetTileRect() const				{ return mTileRect; }

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

		void                        SetState(AIState newState);
		const bool					IsJumpedOn() const			{ return mJumpedOn; }

		virtual void                Update(float dt) = 0; //Virtual method used with the CrawlerWeak and strong
	};

#endif