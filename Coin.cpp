#include "Coin.h"
#include "Game.h"

#include <iostream>

Coin::Coin(int x, int y)
: mRenderable(NULL)
, mRect(0, 0, 0, 0)
{
	// get the texture
	GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture("Coin");

	// create the coin animation
	mRenderable = new GG::Renderable(tex, .75f, true);

	// center the screen rect at the specified coordinates
	mRect.x = x - mRenderable->GetWidth() / 2;
	mRect.y = y - mRenderable->GetHeight() / 2;
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
