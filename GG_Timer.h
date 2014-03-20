#ifndef GG_TIMER_H_
#define GG_TIMER_H_

#include <SDL.h>

namespace GG {

/*
================================================================================

Timer class

    This class can be used to keep track of elapsed time.

    Calling the Start method resets the time to 0 and unpauses the Timer.

    Once a Timer is started, the Pause and Unpause methods can be used to
    pause and unpause the timer (duh).

    The GetTime method returns the current elapsed time in seconds.
    
    All of the methods in this class (except the constructor) assume that
    SDL has already been initialized, since we use the SDL_GetTicks function
    to get the system time.  SDL_GetTicks returns the number of milliseconds
    since SDL was initialized.

================================================================================
*/
class Timer {
    Uint32              mTimeOffset;
    Uint32              mPauseTime;
    bool                mPaused;

public:
                        Timer();

    void                Start();

    void                Pause();
    void                Unpause();

    bool                IsPaused() const;

    float               GetTime() const;
};

inline Timer::Timer()
    : mTimeOffset(0)
    , mPauseTime(0)
    , mPaused(true)
{ }


inline void Timer::Start()
{
    mTimeOffset = SDL_GetTicks();
    mPauseTime = 0;
    mPaused = false;
}

inline void Timer::Pause()
{
    if (!mPaused) {
        mPauseTime = SDL_GetTicks();
        mPaused = true;
    }
}

inline void Timer::Unpause()
{
    if (mPaused) {
        mTimeOffset += SDL_GetTicks() - mPauseTime;
        mPaused = false;
    }
}

inline bool Timer::IsPaused() const
{
    return mPaused;
}

inline float Timer::GetTime() const
{
    if (!mPaused) {
        return 0.001f * (SDL_GetTicks() - mTimeOffset);
    } else {
        return 0.001f * mPauseTime;
    }
}

} // end namespace

#endif
