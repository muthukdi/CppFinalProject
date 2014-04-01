#ifndef COIN_H_
#define COIN_H_

#include "GG_Renderable.h"

class Coin
{
	GG::Renderable*         mRenderable;    // animation state
	GG::Rect                mRect;          // screen rect
	int						mSoundDelay;


public:
	Coin(int x, int y);
	~Coin();

	GG::Renderable*         GetRenderable() const   { return mRenderable; }
	const GG::Rect&         GetRect() const         { return mRect; }

	void                    Update(float dt);

	void					SetSoundDelay(int delayTime){ mSoundDelay += delayTime; }
	int						GetSoundDelay(){ return mSoundDelay; }
};

#endif