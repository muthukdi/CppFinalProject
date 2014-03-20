#include "Explosion.h"
#include "Game.h"

#include <iostream>

Explosion::Explosion(int x, int y)
    : mRenderable(NULL)
    , mRect()
    , mTimeToLive(0)
{
    // get the texture
    GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture("Explosion");

    // create the explosion animation
    mRenderable = new GG::Renderable(tex, 1.15f, false);

    // center the screen rect at the specified coordinates
    mRect.x = x - mRenderable->GetWidth() / 2;
    mRect.y = y - mRenderable->GetHeight() / 2;
    mRect.w = mRenderable->GetWidth();
    mRect.h = mRenderable->GetHeight();

    // set time to live to duration of animation
    mTimeToLive = mRenderable->GetDuration();
}

Explosion::~Explosion()
{
    delete mRenderable;
}

void Explosion::Update(float dt)
{
    mRenderable->Animate(dt);

    mTimeToLive -= dt;
}
