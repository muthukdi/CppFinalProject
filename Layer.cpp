#include "Layer.h"
#include "Game.h"

#include <iostream>

Layer::Layer(float x, float y, const std::string texName, const std::string grayTexName)
    : mRenderable(NULL)
{
    GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture(texName);
	GG::Texture* grayTex = Game::GetInstance()->GetTextureManager()->GetTexture(grayTexName);

    mRenderable = new GG::Renderable(tex, grayTex);

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

void Layer::SetGrayscale(bool grayscale)
{
	mRenderable->SetGrayscale(grayscale);
}