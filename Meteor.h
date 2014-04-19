#ifndef METEOR_H_
#define METEOR_H_

#include "GG_Renderable.h"

class Meteor {
    GG::Renderable*         mRenderable;    // animation state
    GG::Rect                mRect;          // screen rect
	double                  mRotAngle;
    double                  mRotSpeed;

public:
                            Meteor(int x, int y, double rotation);
                            ~Meteor();

    GG::Renderable*         GetRenderable()		   { return mRenderable; }
    const GG::Rect&         GetRect() const         { return mRect; }

    void                    Update(float dt);

	void					SetGrayscale(bool grayscale);
};

#endif