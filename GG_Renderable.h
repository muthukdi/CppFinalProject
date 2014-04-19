#ifndef GG_RENDERABLE_H_
#define GG_RENDERABLE_H_

#include "GG_Graphics.h"
#include "GG_Common.h"

namespace GG {

/*
================================================================================

Renderable class

    A Renderable is something that can be drawn, a.k.a. rendered.

    In this case, a Renderable holds a pointer to a texture, a pointer to 
	a grayscale version of that texture, and the 
    sub-rectangle in these textures where its image resides.

    The Renderable doesn't own the Texture, since multiple renderables can
    share the same Texture object at the same time.  You can think of a
    Renderable as an instance of some graphical resource.  Renderables 
    serve as the visual appearance of game objects.

    This class supports both animated and static (non-animated) Renderables,
    and provides different constructors for both (see comments in code).

    It might help to study the Texture class before the Renderable class,
    since the Renderable class relies heavily on the ideas encapsulated in
    the Texture class (like texture cells, etc.)

================================================================================
*/
class Renderable {

    const Texture*          mTex;           // sprite sheet (we don't own this)
	const Texture*			mGrayscaleTex;  // grayscale version
	bool					mGrayscale;

    int                     mNumFrames;     // number of animation frames or 1 if not animatable

    float                   mDuration;      // total duration in seconds (animated renderable only)
    bool                    mLoopable;      // restart when finished? (animated renderables only)
    float                   mTime;          // current time position (animated renderables only)

    Rect                    mFrameRect;     // current frame rect (cell in sprite sheet)

	double                  mRotAngle;      // rotation angle in degrees
	Point                   mRotOrigin;     // rotation origin

public:
                            Renderable(const Texture* tex, const Texture* grayscaleTex);
                            Renderable(const Texture* tex, const Texture* grayscaleTex, int cellNo);
                            Renderable(const Texture* tex, const Texture* grayscaleTex, float duration, bool loopable);

    const Texture*          GetTexture() const      { return mGrayscale ? mGrayscaleTex : mTex; }
    const Rect*             GetRect() const         { return &mFrameRect; }

    int                     GetWidth() const        { return mFrameRect.w; }
    int                     GetHeight() const       { return mFrameRect.h; }

    bool                    IsAnimatable() const    { return mNumFrames > 1; }

    int                     GetNumFrames() const    { return mNumFrames; }
    float                   GetDuration() const     { return mDuration; }
    bool                    IsLoopable() const      { return mLoopable; }

    bool                    IsAnimating() const     { return mTime < mDuration; }

    void                    Rewind()                { mTime = 0.0f; }

    void                    Animate(float dt);

	double                  GetRotationAngle() const        { return mRotAngle; }
    void                    SetRotationAngle(double angle)  { mRotAngle = angle; }

	const Point&            GetRotationOrigin() const               { return mRotOrigin; }
    void                    SetRotationOrigin(const Point& origin)  { mRotOrigin = origin; }

	void					SetGrayscale(bool grayscale)	{ mGrayscale = grayscale; }
};

} // end of namespace

#endif
