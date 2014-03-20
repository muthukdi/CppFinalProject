#include "Layer.h"
#include "Game.h"

#include <iostream>

Layer::Layer(float x, float y, const std::string name)
    : mRenderable(NULL)
    , mPosX(x)
    , mPosY(y)
{
    GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture(name);

    mRenderable = new GG::Renderable(tex);

    // center the layer at the specified coordinate
    mRect.x = (int)x - mRenderable->GetWidth() / 2;
    mRect.y = (int)y - mRenderable->GetHeight() / 2;
    mRect.w = mRenderable->GetWidth();
    mRect.h = mRenderable->GetHeight();
}

Layer::~Layer()
{
    delete mRenderable;
}
