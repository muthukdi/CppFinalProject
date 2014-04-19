#ifndef LAYER_H_
#define LAYER_H_

#include "GG_Renderable.h"

/*
================================================================================

Layer class

    A Layer just represents a static entity.  It has no update method which
	means that it doesn't do anything except appear on the screen.  It still has
	a rectangle which means that it can experience collisions.

================================================================================
*/

class Layer {

    GG::Renderable*         mRenderable;
    GG::Rect                mRect;

public:
                            Layer(float x, float y, float w, float h, const std::string texName, const std::string grayTexName);
                            ~Layer();

    GG::Renderable*         GetRenderable()		   { return mRenderable; }
    const GG::Rect&         GetRect() const         { return mRect; }

	void					SetGrayscale(bool grayscale);
};

#endif
