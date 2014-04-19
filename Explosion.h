#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "GG_Renderable.h"

class Explosion {
    GG::Renderable*         mRenderable;    // animation state
    GG::Rect                mRect;          // screen rect

    float                   mTimeToLive;    // remaining time to live (in seconds)

public:
                            Explosion(int x, int y);
                            ~Explosion();

    GG::Renderable*         GetRenderable()		   { return mRenderable; }
    const GG::Rect&         GetRect() const         { return mRect; }

    bool                    IsFinished() const      { return mTimeToLive <= 0; }

    void                    Update(float dt);

	void					SetGrayscale(bool grayscale);
};

#endif
