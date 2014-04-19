#include "Meteor.h"
#include "Game.h"

#include <iostream>

Meteor::Meteor(int x, int y, double rotation)
    : mRenderable(NULL)
    , mRect()
	, mRotAngle(0.0)
    , mRotSpeed(rotation)   // degrees per second
{
    // get the texture
    GG::Texture* tex = Game::GetInstance()->GetTextureManager()->GetTexture("Meteor");
	GG::Texture* grayTex = Game::GetInstance()->GetTextureManager()->GetTexture("MeteorGray");

    // create the explosion animation
    mRenderable = new GG::Renderable(tex, grayTex);

    // center the screen rect at the specified coordinates
    mRect.x = x - mRenderable->GetWidth() / 2;
    mRect.y = y - mRenderable->GetHeight() / 2;
    mRect.w = mRenderable->GetWidth();
    mRect.h = mRenderable->GetHeight();
}

Meteor::~Meteor()
{
    delete mRenderable;
}

void Meteor::Update(float dt)
{
	const float speed = 200;  // in pixels per second
    mRect.y += dt * speed;
	mRotAngle += dt * mRotSpeed;
    mRenderable->SetRotationAngle(mRotAngle);
}

void Meteor::SetGrayscale(bool grayscale)
{
	mRenderable->SetGrayscale(grayscale);
}