#ifndef LAYER_H_
#define LAYER_H_

#include "GG_Renderable.h"

class Layer {

    GG::Renderable*         mRenderable;
    GG::Rect                mRect;

public:
                            Layer(float x, float y, const std::string texName, const std::string grayTexName);
                            ~Layer();

    GG::Renderable*         GetRenderable()		   { return mRenderable; }
    const GG::Rect&         GetRect() const         { return mRect; }

	void					SetGrayscale(bool grayscale);
};

#endif
