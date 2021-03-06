#ifndef GAME_H_
#define GAME_H_

#include "GG_Graphics.h"
#include "GG_Timer.h"
#include "Explosion.h"
#include "Grid.h"
#include "Crawler.h"
#include "Robot.h"
#include "Layer.h"
#include "Label.h"
#include "Coin.h"
#include "Meteor.h"
#include "CrawlerStrong.h"
#include "CrawlerWeak.h"

#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <list>

/*
================================================================================

Game class

    This class takes care of the overall game tasks:
    - setting up a window, renderer, and resource managers
    - input handling and basic game logic
    - main loop: the Update and Draw methods get called once per frame
    - keeping track of elapsed time

    The class is a singleton, i.e., there can only be one Game object
    during the lifetime of the application.  This is accomplished by
    making all the constructors private.  Clients can get a pointer
    to the single Game instance by calling the static method Game::GetInstance().

    To create and start a game, the main function can do something like this:

    Game::GetInstance()->Run();

    The Run method then takes over and performs initialization, runs the main loop,
    and cleans up when the game is finished.

================================================================================
*/
class Game {

    SDL_Window*             mWindow;
    SDL_Renderer*           mRenderer;
    const Uint8*            mKeyState;

    int                     mScrWidth;
    int                     mScrHeight;

    GG::TextureManager*     mTexMgr;

    bool                    mShouldQuit;

    GG::Timer               mTimer;
    float                   mTime;          // time elapsed since game started (in seconds)
	float					mMeteorTime;	// time that the last meteor was created (in seconds)
	float					mFlashTime;		// time that the last grayscale/color switch occured
	float					mFlashesNeeded;  // number of flashes that are needed

    Grid*                   mGrid;

	Robot*					mRobot;
	Layer*					mBackground;
	Layer*					mForeground;
	Layer*					mFlagPole;
	Label*					mPointsLabel;
	Label*					mLivesLabel;

	std::list<Explosion*>   mExplosions;
	std::list<Meteor*>		mMeteors;
	std::list<Crawler*>     mCrawlers;
	std::list<Coin*>		mCoins;
	std::list<Layer*>		mMushrooms;

	int						mScene;

	bool					rectVisible;

	Mix_Chunk*				mCoinSound;
	Mix_Chunk*				mJumpSound;
	Mix_Chunk*				mStompSound;
	Mix_Chunk*				mStompSoundNoKill;
	Mix_Chunk*				mDieSound;
	Mix_Chunk*				mBlockSound;
	Mix_Chunk*				mThudSound;
	Mix_Chunk*				mOneupSound;
	Mix_Music*				mMusic;
	Mix_Music*				mGoodGameOverMusic;
	Mix_Music*				mBadGameOverMusic;

	int						mPoints;

public:
    static Game*            GetInstance();

    void                    Run();

    int                     GetScrWidth() const				{ return mScrWidth; }
    int                     GetScrHeight() const			{ return mScrHeight; }

    GG::TextureManager*     GetTextureManager() const		{ return mTexMgr; }

    float                   GetTime() const					{ return mTime; }
	float                   GetMeteorTime() const       { return mMeteorTime; }

    Grid*                   GetGrid() const					{ return mGrid; }
	Robot*					GetRobot() const				{ return mRobot; }
	int						GetScene() const				{ return mScene; }
	Layer*					GetFlagPole() const				{ return mFlagPole; }

    bool                    IsKeyDown(SDL_Scancode s)		{ return mKeyState[s] != 0; }
	void					PlaySound(std::string name);
	void					StopSounds();

	std::list<Crawler*>*	GetCrawlers()					{ return &mCrawlers; }
	std::list<Coin*>*		GetCoins()						{ return &mCoins; }
	std::list<Layer*>*		GetMushrooms()					{ return &mMushrooms; }
	void					SetScene(int scene)				{ mScene = scene; }
	void					LoadScene(int scene, bool items);
	void					LoadTextures();
	void					LoadGrayscaleTextures();
	void					LoadSounds();
	void					SetEntitiesGrayscale(bool grayscale);
	void					SetFlashesNeeded(int flashes)	{ mFlashesNeeded = (float)flashes*2.0f; }

private:
                            Game();
                            ~Game();

                            Game(const Game&);
    Game&                   operator= (const Game&);

    bool                    Initialize();
    void                    Shutdown();

    void                    HandleEvent(const SDL_Event& e);

    void                    Update(float dt);
    void                    Draw();

private:
    void					Render(const GG::Renderable* renderable, const GG::Rect* dstRect, SDL_RendererFlip flip);
};

#endif
