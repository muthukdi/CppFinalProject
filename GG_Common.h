#ifndef GG_COMMON_H_
#define GG_COMMON_H_

#include <cstdlib>

#include <SDL.h>

namespace GG {

/*
================================================================================

Rect class

    Use the Rect class wherever an SDL_Rect structure would be used.

    This class inherits all members of SDL_Rect and keeps them publicly
    visible.  The benefit of using Rect over SDL_Rect is that Rect adds
    constructors.

    It's a bit hacky, but it works, and it's very convenient.

================================================================================
*/
struct Rect : public SDL_Rect {

    Rect()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    Rect(int x, int y, int w, int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

/*
================================================================================

Point class

    Use the Point class wherever an SDL_Point structure would be used.

    This class inherits all members of SDL_Point and keeps them publicly
    visible.  The benefit of using Point over SDL_Point is that Point adds
    constructors.

    It's a bit hacky, but it works, and it's very convenient.

================================================================================
*/
struct Point : public SDL_Point {

    Point()
    {
        x = 0;
        y = 0;
    }

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

/*
================================================================================

InitRandom

    Functions to initialize the global random number generator.
    Should only be called once at the start of the application, unless
    there is a good reason otherwise.

================================================================================
*/
void InitRandom();                  // initialize using a randomized seed
void InitRandom(unsigned seed);     // initialize using the specified seed

/*
================================================================================

UnitRandom

    Return a float in the range [0, 1)
    
================================================================================
*/
inline float UnitRandom()
{
    return rand() / (RAND_MAX + 1.0f);
}

/*
================================================================================

RandomInt

    Return an int in the range [0, upper)

    Good for returning random array indexes.
    
================================================================================
*/
inline int RandomInt(int upper)
{
    return (int)(upper * UnitRandom());
}

/*
================================================================================

RandomInt

    Return an int in the range [lower, upper)
    
================================================================================
*/
inline int RandomInt(int lower, int upper)
{
    return lower + (int)((upper - lower) * UnitRandom());
}

/*
================================================================================

RandomFloat

    Return a float in the range [lower, upper)
    
================================================================================
*/
inline float RandomFloat(float lower, float upper)
{
    return lower + (upper - lower) * UnitRandom();
}

/*
================================================================================

RandomSign
    
    Return either 1 or -1

================================================================================
*/
inline int RandomSign()
{
    return UnitRandom() < 0.5f ? 1 : -1;
}

/*
================================================================================

UnitRandomInclusive

    Return a float in the range [0, 1]

================================================================================
*/
inline float UnitRandomInclusive()
{
    return rand() / (float)RAND_MAX;
}

/*
================================================================================

RandomIntInclusive

    Return an int in the range [lower, upper]

================================================================================
*/
inline int RandomIntInclusive(int lower, int upper)
{
    return lower + (int)((upper - lower + 1) * UnitRandom());
}

/*
================================================================================

RandomFloatInclusive

    Return a float in the range [lower, upper]

================================================================================
*/
inline float RandomFloatInclusive(float lower, float upper)
{
    return lower + (upper - lower) * UnitRandomInclusive();
}

} // end of namespace

#endif
