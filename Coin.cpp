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

	// center the screen rect at the specified coordinates
	mRect.x = (int)x - mRenderable->GetWidth() / 2;
	mRect.y = (int)y - mRenderable->GetHeight() / 2;
	mRect.w = mRenderable->GetWidth();
	mRect.h = mRenderable->GetHeight();

}


Coin::~Coin()
{
	delete mRenderable;
}

void Coin::Update(float dt)
{
	mRenderable->Animate(dt);

}
