#ifndef ENTITY_H_
#define ENTITY_H_

#include "GG_Renderable.h"
#include <iostream>


class Entity
{
protected:
	GG::Renderable*         mRenderable;
	GG::Rect				mRect;
	float                   mPosX;
	float                   mPosY;


public:
	Entity(float x, float y)
		: mRenderable(NULL)
		, mRect(0,0,0,0)
		, mPosX(x)
		, mPosY(y)
	{}

	virtual ~Entity(){}
	GG::Renderable*         GetRenderable() const				{ return mRenderable; }
	const GG::Rect&			GetRect() const						{ return mRect; }
	virtual void			Update(float dt)					{ std::cout << "Entity called Update" << std::endl;}
};

#endif