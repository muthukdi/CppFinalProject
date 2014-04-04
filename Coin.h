#ifndef COIN_H_
#define COIN_H_

#include "GG_Renderable.h"
#include "Entity.h"

class Coin : public Entity
{
	//GG::Renderable*         mRenderable;    // animation state
	//GG::Rect                mRect;          // screen rect
	//float                   mPosX;
	//float                   mPosY;
	int						mSoundDelay;


public:
	Coin(float x, float y);
	~Coin();

	//GG::Renderable*         GetRenderable() const   { return mRenderable; }
	//const GG::Rect&         GetRect() const         { return mRect; }

	void                    Update(float dt);

	void					SetSoundDelay(int delayTime){ mSoundDelay += delayTime; }
	int						GetSoundDelay(){ return mSoundDelay; }
};

#endif