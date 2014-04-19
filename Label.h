#ifndef LABEL_H_
#define LABEL_H_

#include "GG_Renderable.h"

class Label {

    GG::Renderable*         mRenderable;
    GG::Rect                mRect;

    float                   mPosX;
    float                   mPosY;

public:
                            Label(float x, float y, const std::string name);
                            ~Label();

    GG::Renderable*         GetRenderable()		   { return mRenderable; }
    const GG::Rect&         GetRect() const         { return mRect; }
};

#endif