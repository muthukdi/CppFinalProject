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
	, mCoin(NULL)
	, mScene(0)
	, rectVisible(0)
	, mCoinSound(NULL)
	, mJumpSound(NULL)
	, mStompSound(NULL)
	, mDieSound(NULL)
	, mMusic(NULL)
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

	//Initialize SDL Audio
	if (SDL_INIT_AUDIO < 0)
	{
		std::cerr << "SDL audio not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}

	//Load musicIn
	mMusic = Mix_LoadMUS("media/music.wav");
	if (mMusic == NULL)
	{
		std::cerr << " Failed to load beat music! SDL_mixer Error:" << Mix_GetError() << std::endl;
		return false;
	}
	//loading the Coin Sound
	mCoinSound = Mix_LoadWAV("media/coin_sound.wav");
	mJumpSound = Mix_LoadWAV("media/jump_sound.wav");
	mStompSound = Mix_LoadWAV("media/stomp_sound.wav");
	mDieSound = Mix_LoadWAV("media/die_sound.wav");
	if (mCoinSound == NULL || mJumpSound == NULL || mStompSound == NULL || mDieSound == NULL)
	{
		std::cerr << "*** Failed to initialize mCoinSound" << Mix_GetError()<<std::endl;
		return false;
	}

    // load textures
	mTexMgr->LoadTexture("Background2", "image.png");
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
	mTexMgr->LoadTexture("Coin", "coin.png", 10);

    // initialize grid
    mGrid = CreateRandomLevel("Tiles");

	mCoin = new Coin(mScrWidth - mScrWidth *.33f, mScrHeight - mScrHeight *.33f);
	mRobot = new Robot(mScrWidth* .5f, mScrHeight-160.0f);
	mBackground = new Layer(mScrWidth * .5f, mScrHeight * .5f, "Background");
	mForeground = new Layer(mScrWidth * .5f, mScrHeight * .5f, "Foreground");

	// create a bunch of crawlers at random locations and random orientations
 /*   float minX = 32;
    float maxX = mScrWidth - 32.0f;
    float y = mScrHeight - 1.0f - 32.0f;
    for (int i = 0; i < 3; i++)
	{
        float x = GG::RandomFloat(minX, maxX);
        Crawler* crawler = new Crawler(x, y);
        crawler->SetDirection(GG::RandomSign());
        mCrawlers.push_back(crawler);
    }*/
	
	Mix_PlayMusic(mMusic, -1);
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
	//Frees the sound chunks and sets it to NULL
	Mix_FreeChunk(mCoinSound);
	mCoinSound = NULL;
	Mix_FreeChunk(mJumpSound);
	mJumpSound = NULL;
	Mix_FreeChunk(mStompSound);
	mStompSound = NULL;
	Mix_FreeChunk(mDieSound);
	mDieSound = NULL;

	//Free the music
	Mix_FreeMusic(mMusic);
	mMusic = NULL;

	//We also need to quit the mixer
	Mix_Quit();

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

	// delete all crawlers and clear the list
    std::list<Crawler*>::iterator crawlerIter = mCrawlers.begin();
    for ( ; crawlerIter != mCrawlers.end(); ++crawlerIter)
	{
        Crawler* crawler = *crawlerIter;
        delete crawler;
    }
    mCrawlers.clear();

    // delete the texture manager (and all the textures it loaded for us)
    delete mTexMgr;
    mTexMgr = NULL;

	delete mCoin;
	mCoin = NULL;

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
		case SDLK_v:
			//
			// show/hide collision rectangle
			//
			if (rectVisible)
			{
				rectVisible = 0;
			}
			else
			{
				rectVisible = 1;
			}
			break;
		case SDLK_9:
			//If there is no music playing
			if (Mix_PlayingMusic() == 0)
			{
				//Play the music
				Mix_PlayMusic(mMusic, -1);
			}
			//If music is being played
			else
			{
				//If the music is paused
				if (Mix_PausedMusic() == 1)
				{
					//Resume the music
					Mix_ResumeMusic();
				}
				//If the music is playing
				else
				{
					//Pause the music
					Mix_PauseMusic();
				}
			}
			break;
		case SDLK_c:
			// Add a crawler
			float x = GG::RandomFloat(32, mScrWidth - 32.0f);
			Crawler* crawler = new Crawler(x, mScrHeight - 1.0f - 32.0f);
			crawler->SetDirection(GG::RandomSign());
			mCrawlers.push_back(crawler);
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
	// Update the robot
	if (mRobot)
	{
		mRobot->Update(dt);
		// Check if the robot collides with the coin
		if (mCoin)
		{
			if (mRobot->GetCollisonRect().y < mCoin->GetRect().y + mCoin->GetRect().h)
			{
				if ((mRobot->GetCollisonRect().x < mCoin->GetRect().x + mCoin->GetRect().w)
 					&& (mRobot->GetCollisonRect().x + mRobot->GetCollisonRect().w >mCoin->GetRect().x))
				{
					//I have found that the sound is delayed...So I start it a bit earlier than the actualy delete of the Coin
					if (mCoin->GetSoundDelay() == 0)
					{
						Mix_PlayChannel(-1, mCoinSound, 0);
						mCoin->SetSoundDelay(1);
					}
					//Once it has run through 4 times then it Deletes the coin
					else if (mCoin->GetSoundDelay() == 5)
					{
						delete mCoin;
						mCoin = NULL;
					}
					else
					{
						mCoin->SetSoundDelay(1);
					}
				}
			}
		}
	}

	// update all crawlers
	std::list<Crawler*>::iterator crawlerIt = mCrawlers.begin();
    while (crawlerIt != mCrawlers.end())
	{
		Crawler *crawler = *crawlerIt;
		if (crawler->GetState() == Crawler::CRAWLER_DEAD)
		{
			crawlerIt = mCrawlers.erase(crawlerIt); // remove the entry from the list and advance iterator
            delete crawler;              // delete the object
		}
		else
		{
			// If the robot is falling from a jump
			if (mRobot->GetVerticalVelocity() > 0.0 && mRobot->GetJumping())
			{
				// Check if the robot has started squashing the poor crawler
				if (mRobot->GetCollisonRect().x + mRobot->GetCollisonRect().w > crawler->GetCollisionRect().x && 
				mRobot->GetCollisonRect().x < crawler->GetCollisionRect().x + crawler->GetCollisionRect().w &&
				mRobot->GetCollisonRect().y + mRobot->GetCollisonRect().h > crawler->GetCollisionRect().y && crawler->GetState() != Crawler::CRAWLER_DYING)
				{
					Mix_PlayChannel(-1, mStompSound, 0);
					mRobot->Bounce(-400, false);
					crawler->SetState(Crawler::CRAWLER_DYING);
				}
			}
			// If the robot runs into a crawler, the robot must die (but it should not be jumping at this time)
			else if (mRobot->GetCollisonRect().x + mRobot->GetCollisonRect().w > crawler->GetCollisionRect().x && 
				mRobot->GetCollisonRect().x < crawler->GetCollisionRect().x + crawler->GetCollisionRect().w &&
				!mRobot->IsDead() && !mRobot->GetJumping() && crawler->GetState() != Crawler::CRAWLER_DYING)
			{
				Mix_PlayChannel(-1, mDieSound, 0);
				mRobot->Bounce(-400, true);             // kill the robot
			}
			crawler->Update(dt);
			++crawlerIt;
		}
    }

	// Update the coin
	if (mCoin)
	{
		mCoin->Update(dt);
	}
	else
	{
		float minX = 32;
		float maxX = mScrWidth - 32.0f;
		float x = GG::RandomFloat(minX, maxX);
		mCoin = new Coin(x, mScrHeight - mScrHeight *.33f);
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
	if (mRobot->GetRect().x  > mScrWidth)
	{
		mScene++;
		// Change the background image when entering a new scene
		if (mScene % 2 == 0)
		{
			//std::cout << mScene << " is odd  " << std::endl;
			delete mBackground;
			mBackground = NULL;
			mBackground = new Layer(mScrWidth *.5f, mScrHeight *.5f, "Background");
		}
		else
		{
			//std::cout << mScene << " is even  " << std::endl;
			delete mBackground;
			mBackground = NULL;
			mBackground = new Layer(mScrWidth *.5f, mScrHeight *.5f, "Background2");

		}
		
	}
	
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

	// Draw the collision rectangles
	if (rectVisible)
	{
		// set new color for drawing
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);
		// draw the player sprite using the selected color
		SDL_RenderFillRect(mRenderer, &mRobot->GetCollisonRect());
		SDL_RenderFillRect(mRenderer, &mCoin->GetRect());
		for (auto crawlerIt = mCrawlers.begin(); crawlerIt != mCrawlers.end(); ++crawlerIt)
		{
			Crawler* crawler = *crawlerIt;
			SDL_RenderFillRect(mRenderer, &crawler->GetCollisionRect());
		}
	}
	//
    // draw the robot
    //
	if (mRobot)
	{
			Render(mRobot->GetRenderable(), &mRobot->GetRect(), mRobot->GetDirection()?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
	}

	//
    // draw the coin
    //
	if (mCoin)
	{
		Render(mCoin->GetRenderable(), &mCoin->GetRect(), SDL_FLIP_NONE);
	}

	//
    // draw the crawlers
    //
    for (auto crawlerIt = mCrawlers.begin(); crawlerIt != mCrawlers.end(); ++crawlerIt)
	{
        Crawler* crawler = *crawlerIt;
		if (crawler->GetDirection() == 1)
		{
			Render(crawler->GetRenderable(), &crawler->GetRect(), SDL_FLIP_HORIZONTAL);
		}
		else
		{
			Render(crawler->GetRenderable(), &crawler->GetRect(), SDL_FLIP_NONE);
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

void Game::PlaySound(std::string name)
{
	Mix_PlayChannel(-1, mJumpSound, 0);
}
