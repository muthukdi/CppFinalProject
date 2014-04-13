#ifndef GG_GRAPHICS_H_
#define GG_GRAPHICS_H_

#include <SDL.h>
#include <string>
#include <map>
#include <SDL_ttf.h>

namespace GG {

/*
================================================================================

Image class

    This class is used to store raw image data in memory.  Internally, it
    stores the information in an SDL_Surface struct, which it owns.

    Images can be loaded from files using the Load method.

    "Blank" images can be allocated using the Alloc method.  The contents of
    such images are not initialized in any way.  The class allows access to
    the image data using a pointer, so that an external method can be used
    to procedurally fill in the image contents.

    See the TextureManager::CreateDefaultTexture method for an example of
    procedurally generating image data.

    Images are not used for drawing directly, they merely hold the image 
    data in memory.  The TextureManager class can be used to create a Texture
    object from an Image, which can be used for drawing.

================================================================================
*/
class Image {

    SDL_Surface*            mSurface;

public:
                            Image();
                            Image(const std::string& path);
                            ~Image();

    bool                    Load(const std::string& path);
    bool                    Alloc(int width, int height, int bytesPerPixel);

    void                    Unload();

    bool                    IsLoaded() const    { return mSurface != NULL; }

    SDL_Surface*            GetSurface() const  { return mSurface; }

    int                     GetWidth() const    { return mSurface ? mSurface->w : 0; }  // width in pixels
    int                     GetHeight() const   { return mSurface ? mSurface->h : 0; }  // height in pixels

    int                     GetPitch() const    { return mSurface ? mSurface->pitch : 0; }  // number of bytes per row of pixels

    Uint8*                  GetData()           { return mSurface ? (Uint8*)mSurface->pixels : NULL; }  // pointer to raw image data

    Uint8*                  GetRow(int row)     { return GetData() + row * GetPitch(); }  // pointer to the beginning of a row of pixels
};

/*
================================================================================

Texture class

    Represents a drawable graphics resource.  Internally, the texture
    is represented using the SDL_Texture structure.  The texture data
    is typically stored in GPU memory (DRAM).

    This class provides an easy way to get information about the texture,
    such as width and height.

    It also defines the layout of cells within the texture.  A texture may
    have more than one logical cell, such as sprite animations composed
    of multiple frames.

    This class assumes that all cells are arranged in a single horizontal row
    and that all cells are the same size.  Clients can query the number of
    cells and cell dimensions using accessor methods.

    All textures must be assigned a name, which acts as a unique identifier.
    The TextureManager class keeps a lookup table of Texture objects and 
    allows clients to retrieve Textures by name.

    The Texture constructors take a pointer to the SDL_Texture struct
    which is assumed to be already initialized.  This liberates this class
    from worrying about how textures are actually created (whether they're
    loaded from files, generated procedurally, or whatever).

    All of that the texture loading is done using the TextureManager class,
    which constructs a Texture object once it creates the SDL_Texture by
    whatever means necessary.

    Once a Texture object is constructed, it takes ownership of the 
    SDL_Texture passed in the constructor.  That means that the Texture
    object becomes responsible for deleting the SDL_Texture, which happens
    in the destructor.

================================================================================
*/
class Texture {

    std::string             mName;          // unique string identifier

    SDL_Texture*            mTex;           // pointer to texture implementation

    int                     mWidth;
    int                     mHeight;

    int                     mNumCells;      // total number of cells (1 or more)

    int                     mCellWidth;
    int                     mCellHeight;

public:
                            Texture(const std::string& name, SDL_Texture* tex);  // create a single-cell texture
                            Texture(const std::string& name, SDL_Texture* tex, int numCells);  // create a multi-cell texture
                            ~Texture();

    const std::string&      GetName() const         { return mName; }

    SDL_Texture*            GetPtr() const          { return mTex; }

    int                     GetWidth() const        { return mWidth; }
    int                     GetHeight() const       { return mHeight; }

    int                     GetNumCells() const     { return mNumCells; }

    int                     GetCellWidth() const    { return mCellWidth; }
    int                     GetCellHeight() const   { return mCellHeight; }
};

/*
================================================================================

TextureManager

    Takes care of loading textures and making them available to the rest of the
    application.

    Typically, there would be one TextureManager per game, but this class
    doesn't impose any such restrictions (no singleton silliness here).

    The LoadTexture methods take care of loading and initializing textures.
    All texture loading should be done during game intialization, before
    the main loop commences.  Each texture should be loaded only once.
    
    The TextureManager can be initialized with the path of the root directory
    where the texture images reside.  All image paths are assumed to be relative
    to that directory.

    Each texture must be given a unique name at load time.  The texture
    manager maintains a lookup table that allows clients to retrieve textures
    by name using the GetTexture method. This helps to insulate the high-level
    game logic layers from the itty-gritty details of where the textures come
    from and how they are loaded.

    If a client requests a non-existing texture, the texture manager will
    return a pointer to a default texture, which it generates procedurally
    during initialization.  This is better than just returning NULL pointers,
    which are a pain to deal with and can result in crashes if not dealt with
    properly.  The default texture is a colored checkerboard which can be 
    easily noticed while testing the game, so it also serves as a visual
    debugging tool of sorts.

    The TextureManager owns all of the Texture objects that it creates
    and is solely responsible for deleting them.  In other words, no one else
    should be deleting the Texture objects that the TextureManager returns.

================================================================================
*/
class TextureManager {

    SDL_Renderer*           mRenderer;
    std::string             mRootDir;

    std::map<std::string, Texture*> mTextures;

    Texture*                mDefaultTex;
	TTF_Font*				font;

    Texture*                CreateDefaultTexture();

public:
                            TextureManager();
                            ~TextureManager();

    bool                    Initialize(SDL_Renderer* renderer, const std::string& rootDir);

    Texture*                LoadTexture(const std::string& name, const std::string& filename, int numCells = 1);
    Texture*                LoadTexture(const std::string& name, const Image& img, int numCells = 1);
	Texture*				LoadTexture(const std::string& name, const char* text, SDL_Color text_color);

    Texture*                GetTexture(const std::string& name) const;

    Texture*                GetDefaultTexture() const       { return mDefaultTex; }

    void                    DeleteTexture(const std::string& name);
    void                    DeleteTexture(Texture* tex);

    void                    DeleteAll();

private:
	int						Grayscale(SDL_Surface *image);
};

} // end namespace

#endif
