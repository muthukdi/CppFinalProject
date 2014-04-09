#include "GG_Renderable.h"

#include <cmath>

namespace GG {

/*
================================================================================

Renderable constructor (non-animated)

    This constructor creates a static (non-animated) renderable using the
    entire specified texture.

================================================================================
*/
Renderable::Renderable(const Texture* tex)
    : mTex(tex)
    , mNumFrames(1)
    , mDuration(0.0f)
    , mLoopable(false)
    , mTime(0.0f)
    , mFrameRect()
	, mRotAngle(0.0)
	, mRotOrigin()
{
    // set the frame rect to cover the entire texture
    if (tex) {
        mFrameRect.x = 0;
        mFrameRect.y = 0;
        mFrameRect.w = tex->GetWidth();
        mFrameRect.h = tex->GetHeight();
		// set rotation origin to rect centroid by default
        mRotOrigin.x = mFrameRect.w / 2;
        mRotOrigin.y = mFrameRect.h / 2;
    }
}

/*
================================================================================

Renderable constructor (non-animated)

    This constructor creates a static (non-animated) renderable using the
    specified cell index in the texture.

================================================================================
*/
Renderable::Renderable(const Texture* tex, int cellNo)
    : mTex(tex)
    , mNumFrames(1)
    , mDuration(0.0f)
    , mLoopable(false)
    , mTime(0.0f)
    , mFrameRect()
	, mRotAngle(0.0)
    , mRotOrigin()
{
    // set the frame rect to the specified cell
    if (tex) {
        mFrameRect.x = cellNo * tex->GetCellWidth();
        mFrameRect.y = 0;
        mFrameRect.w = tex->GetCellWidth();
        mFrameRect.h = tex->GetCellHeight();
		// set rotation origin to rect centroid by default
        mRotOrigin.x = mFrameRect.w / 2;
        mRotOrigin.y = mFrameRect.h / 2;
    }
}

/*
================================================================================

Renderable constructor (animated)

    This constructor creates an animated renderable using all the cells in the
    specified texture.

================================================================================
*/
Renderable::Renderable(const Texture* tex, float duration, bool loopable)
    : mTex(tex)
    , mNumFrames(tex->GetNumCells())
    , mDuration(duration)
    , mLoopable(loopable)
    , mTime(0.0f)
    , mFrameRect()
	, mRotAngle(0.0)
    , mRotOrigin()
{
    // set the frame rect to be the first cell in the texture
    if (tex) {
        mFrameRect.x = 0;
        mFrameRect.y = 0;
        mFrameRect.w = tex->GetCellWidth();
        mFrameRect.h = tex->GetCellHeight();
		// set rotation origin to rect centroid by default
        mRotOrigin.x = mFrameRect.w / 2;
        mRotOrigin.y = mFrameRect.h / 2;
    }
}

/*
================================================================================

Renderable::Animate

    This method can be used to advance the time position of animated renderables
    by the specified time interval (in seconds).

    The method also takes care of animation looping logic and updating the
    frame rect to correspond with the current cell in the sprite sheet texture.

================================================================================
*/
void Renderable::Animate(float dt)
{
    if (!IsAnimatable()) {
        return;
    }

    // update time position
    mTime += dt;

    // see if we've reached or passed the end
    if (mTime >= mDuration) {
        if (mLoopable) {
            // wrap around
            mTime = std::fmod(mTime, mDuration);
        } else {
            // cap
            mTime = mDuration;
        }
    }

    // figure out which frame we're on
    int frameNo;

    if (mTime < mDuration) {
        // select a frame based on time position
        float progress = mTime / mDuration;         // 0.0 <= progress < 1.0
        frameNo = (int)(mNumFrames * progress);     // 0 <= frameNo < mNumFrames
    } else {
        // use last frame
        frameNo = mNumFrames - 1;
    }

    // compute the offset of the current frame rect in the texture
    mFrameRect.x = frameNo * mFrameRect.w;
}

} // end of namespace
