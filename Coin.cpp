#include "Coin.h"
#include "Game.h"

#include <iostream>

Coin::Coin(float x, float y)
: mRenderable(NULL)
, mRect(0,0,0,0)
, mPosX(x)
, mPosY(y)
, mSoundDelay(0)
{
	// get the texture
	GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture("Coin");

	// create the coin animation
	mRenderable = new GG::Renderable(tex, .75f, true);


	mRect.w = mRenderable->GetWidth();
	mRect.h = mRenderable->GetHeight();
	// center the screen rect at the specified coordinates
	mRect.x = (int)(mPosX);
	mRect.y = (int)(mPosY - mRect.h);         // y-coord of position is at the bottom of screen rect

}


Coin::~Coin()
{
	delete mRenderable;
}

void Coin::Update(float dt)
{
	mRenderable->Animate(dt);
}
