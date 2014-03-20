#ifndef LAYER_H_
#define LAYER_H_

#include "GG_Renderable.h"

class Layer {

    GG::Renderable*         mRenderable;
    GG::Rect                mRect;

    float                   mPosX;
    float                   mPosY;

public:
                            Layer(float x, float y, const std::string name);
                            ~Layer();

    GG::Renderable*         GetRenderable() const   { return mRenderable; }
    const GG::Rect&         GetRect() const         { return mRect; }
};

#endif
