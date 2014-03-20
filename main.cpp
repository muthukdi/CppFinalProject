#include "GG_Common.h"

#include "Game.h"

//#include <vld.h>

int main(int argc, char** argv)
{
    // initialize the random number generator
    GG::InitRandom();

    // create and run a Game instance
    Game::GetInstance()->Run();

    return 0;
}
