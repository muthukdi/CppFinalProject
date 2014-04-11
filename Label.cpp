#include "Label.h"
#include "Game.h"

#include <iostream>

Label::Label(float x, float y, const std::string name)
    : mRenderable(NULL)
    , mPosX(x)
    , mPosY(y)
{
    GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture(name);

    mRenderable = new GG::Renderable(tex);

    // Position the top left hand corner of the label at
	// the specified coordinates
    mRect.x = (int)x;
    mRect.y = (int)y;
    mRect.w = mRenderable->GetWidth();
    mRect.h = mRenderable->GetHeight();
}

Label::~Label()
{
    delete mRenderable;
}
