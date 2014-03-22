#include "Game.h"
#include "Level.h"

#include <SDL_image.h>

#include <iostream>

/*
================================================================================

Game constructor

    This constructor just ensures that all members are initialized to known
    values.

    It does not perform any true initialization, since this is deferred to
    the Initialize method that gets called at the beginning of the Run method.

================================================================================
*/
Game::Game()
    : mWindow(NULL)
    , mRenderer(NULL)
    , mKeyState(NULL)
    , mScrWidth(0)
    , mScrHeight(0)
    , mTexMgr(NULL)
    , mShouldQuit(false)
    , mTime(0.0f)
    , mGrid(NULL)
	, mRobot(NULL)
{
}

/*
================================================================================

Game destructor

    Should do nothing.  All cleanup should be performed in the Shutdown method
    that gets called at the end of the Run method.

================================================================================
*/
Game::~Game()
{
}

/*
================================================================================

Game::GetInstance

    Clients call this to get a pointer to the singleton Game instance.
    It is a static method.

================================================================================
*/
Game* Game::GetInstance()
{
    static Game game;
    return &game;
}

/*
================================================================================

Game::Run

    Manages the key stages of the game: initialization, main loop, and cleanup.
    Also takes care of time keeping.

================================================================================
*/
void Game::Run()
{
    // startup
    if (!Initialize())
	{
        std::cerr << "*** Game initialization failed" << std::endl;
        return;
    }

    // start timing
    mTimer.Start();
    mTime = 0.0f;
    
    // main loop
    mShouldQuit = false;
    while (!mShouldQuit)
	{

        // dispatch events
        SDL_Event e;
        while (SDL_PollEvent(&e))
		{
            HandleEvent(e);
        }

        // draw this frame
        Draw();

        // only run update if we're not paused
        if (!mTimer.IsPaused())
		{
            // get time elapsed since last frame
            float now = mTimer.GetTime();
            float dt = now - mTime;
            mTime = now;
            // run game logic for this frame
            Update(dt);
        }
    }

    // cleanup
    Shutdown();
}

/*
================================================================================

Game::Initialize

    Performs the bulk of game initialization.  Creates a window, renderer, and
    resource managers.  Loads resources, spawns entities, and creates the
    initial scene layout.

================================================================================
*/
bool Game::Initialize()
{
    std::cout << "***" << std::endl;
    std::cout << "*** Click to make boom!" << std::endl;
    std::cout << "***" << std::endl;

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        std::cerr << "*** Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // initialize SDL_image add-on
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
	{
        std::cerr << "*** Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    mScrWidth = 640;
    mScrHeight = 480;

    // create a window
    mWindow = SDL_CreateWindow("C++ Final Project",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               mScrWidth, mScrHeight,
                               SDL_WINDOW_SHOWN /*| SDL_WINDOW_RESIZABLE*/);
    if (!mWindow)
	{
        std::cerr << "*** Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    // create a renderer that takes care of drawing stuff to the window
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
	{
        std::cerr << "*** Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // get a pointer to keyboard state managed by SDL
    mKeyState = SDL_GetKeyboardState(NULL);

    // create a texture manager
    mTexMgr = new GG::TextureManager;
    if (!mTexMgr->Initialize(mRenderer, "media/"))
	{
        std::cerr << "*** Failed to initialize texture manager" << std::endl;
        return false;
    }

    // load textures
    mTexMgr->LoadTexture("Background", "Layer0.png");
	mTexMgr->LoadTexture("Foreground", "Layer1.png");
    mTexMgr->LoadTexture("Tiles", "tiles.tga", 7);
    mTexMgr->LoadTexture("Explosion", "explosion.tga", 16);
	mTexMgr->LoadTexture("RobotIdle", "robot_idle.png", 8);
	mTexMgr->LoadTexture("RobotRun", "robot_run.png", 6);
	mTexMgr->LoadTexture("RobotJump", "robot_jump.png", 8);
	mTexMgr->LoadTexture("RobotDie", "robot_die.png", 8);
	mTexMgr->LoadTexture("Meteor", "meteor.png");
	mTexMgr->LoadTexture("CrawlerWalk", "crawler_walk.png", 8);
    mTexMgr->LoadTexture("CrawlerIdle", "crawler_idle.png", 8);
	mTexMgr->LoadTexture("CrawlerDie", "crawler_die.png", 8);

    // initialize grid
    mGrid = CreateRandomLevel("Tiles");

	mBackground = new Layer(mScrWidth / 2, mScrHeight / 2, "Background");
	mForeground = new Layer(mScrWidth / 2, mScrHeight / 2, "Foreground");
	mRobot = new Robot(mScrWidth / 2, mScrHeight-128-32);

    return true;
}

/*
================================================================================

Game::Shutdown

    Performs cleanup after the game ends.  Deletes any active entities,
    releases resources, kills the window, and shuts down the supporting libs.

================================================================================
*/
void Game::Shutdown()
{
	delete mRobot;
	mRobot = NULL;

    // delete grid
    delete mGrid;
    mGrid = NULL;

    // delete all explosions
    std::list<Explosion*>::iterator it = mExplosions.begin();
    for ( ; it != mExplosions.end(); ++it)
	{
        delete *it;
    }
    mExplosions.clear();

    // delete the texture manager (and all the textures it loaded for us)
    delete mTexMgr;
    mTexMgr = NULL;

	delete mBackground;
	mBackground = NULL;
	delete mForeground;
	mForeground = NULL;

    // unload the image libraries
    IMG_Quit();

    // shut down SDL (deletes the window and renderer)
    SDL_Quit();
    mWindow = NULL;
    mRenderer = NULL;
    mKeyState = NULL;
}

/*
================================================================================

Game::HandleEvent

    Processes discrete events generated by the keyboard, mouse, window, etc.

================================================================================
*/
void Game::HandleEvent(const SDL_Event& e)
{
    switch (e.type)
	{
    case SDL_QUIT:
        mShouldQuit = true;
        break;

    case SDL_WINDOWEVENT:
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
		{
            mScrWidth = e.window.data1;
            mScrHeight = e.window.data2;
        }
        break;

    case SDL_KEYDOWN:
        switch (e.key.keysym.sym)
		{
        case SDLK_ESCAPE:
            mShouldQuit = true;
            break;

        case SDLK_r:
            //
            // repopulate the grid
            //
            delete mGrid;
            mGrid = CreateRandomLevel("Tiles");
            break;

        case SDLK_p:
            //
            // pause or unpause
            //
            if (mTimer.IsPaused())
			{
                mTimer.Unpause();
            } else
			{
                mTimer.Pause();
            }
            break;
        }
        break;
    }
}

/*
================================================================================

Game::Update

    Gets called once per frame to perform game logic.
    The parameter dt is the time elapsed since last frame (in seconds).

================================================================================
*/
void Game::Update(float dt)
{
    
	if (mRobot)
	{
		mRobot->Update(dt);
	}

    //
    // update the explosions
    //
    std::list<Explosion*>::iterator it = mExplosions.begin();
    while (it != mExplosions.end())
	{

        Explosion* entity = *it;        // get a pointer to this explosion

        if (entity->IsFinished())
		{
            it = mExplosions.erase(it); // remove the entry from the list and advance iterator
            delete entity;              // delete the object
        } 
		else
		{
            entity->Update(dt);     // update the entity
            ++it;                   // advance list iterator
        }
    }
}

/*
================================================================================

Game::Draw

    Gets called once per frame to draw the current snapshot of the game.

================================================================================
*/
void Game::Draw()
{
    // clear the screen
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

	if (mBackground)
	{
		Render(mBackground->GetRenderable(), &mBackground->GetRect(), SDL_FLIP_NONE);
	}
	if (mForeground)
	{
		Render(mForeground->GetRenderable(), &mForeground->GetRect(), SDL_FLIP_NONE);
	}

    //
    // draw the grid
    //
    if (mGrid)
	{
        int tileWidth = mGrid->TileWidth();
        int tileHeight = mGrid->TileHeight();
        GG::Rect tileRect(0, mScrHeight-tileHeight, tileWidth, tileHeight);
        for (int x = 0; x < mGrid->NumCols(); x++)
		{
            GG::Renderable* renderable = mGrid->GetTile(mGrid->NumRows()-1, x)->GetRenderable();
            if (renderable) 
			{
                Render(renderable, &tileRect, SDL_FLIP_NONE);
            }
            tileRect.x += tileWidth;
        }
    }

	//
    // draw the robot
    //
	if (mRobot)
	{
		if (mRobot->GetJumping() == 1)
		{
			Render(mRobot->GetRenderableJump(), &mRobot->GetRect(), mRobot->GetDirection()?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
		}
		else if (IsKeyDown(SDL_SCANCODE_A) || IsKeyDown(SDL_SCANCODE_D))
		{
			Render(mRobot->GetRenderableRun(), &mRobot->GetRect(), mRobot->GetDirection()?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
		}
		else
		{
			Render(mRobot->GetRenderableIdle(), &mRobot->GetRect(), mRobot->GetDirection()?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
		}
	}

    //
    // draw the explosions
    //
    std::list<Explosion*>::iterator it = mExplosions.begin();
    for ( ; it != mExplosions.end(); ++it)
	{
        Explosion* boom = *it;
        Render(boom->GetRenderable(), &boom->GetRect(), SDL_FLIP_NONE);
    }

    // display everything we just drew
    SDL_RenderPresent(mRenderer);
}


/*
================================================================================

Game::Render

    A helper method for drawing a Renderable into a destination rectangle
    on the screen.  It gets the Renderable's texture and source rectangle
    in the texture and forwards everything to the SDL_RenderCopy function.

    One purpose of this method is to save some typing and improve
    readability of drawing code in the Game::Draw method.

    Additionally, this method provides an extra check for NULL Renderable
    pointers, which helps to keep shit game code afloat a while longer.
    In case of a NULL Renderable, this method will use the TextureManager's
    default texture instead.

================================================================================
*/
void Game::Render(const GG::Renderable* renderable, const GG::Rect* dstRect, SDL_RendererFlip flip)
{
    if (renderable)
	{
        SDL_RenderCopyEx(mRenderer, 
					renderable->GetTexture()->GetPtr(), 
					renderable->GetRect(), 
					dstRect, 
					0.0, 
					NULL, 
					flip);
    }
	else
	{
        SDL_RenderCopy(mRenderer,
                       mTexMgr->GetDefaultTexture()->GetPtr(),
                       NULL,
                       dstRect);
    }
}
