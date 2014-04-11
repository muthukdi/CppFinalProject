#include "GG_Graphics.h"

#include <SDL_image.h>
#include <iostream>

namespace GG {

/*
================================================================================

Image default constructor

    Creates an empty image.

================================================================================
*/
Image::Image()
    : mSurface(NULL)
{
}

/*
================================================================================

Image constructor

    Loads an image from the specified path.

================================================================================
*/
Image::Image(const std::string& path)
    : mSurface(NULL)
{
    Load(path);
}

/*
================================================================================

Image destructor

    Ensures the image gets unloaded.

================================================================================
*/
Image::~Image()
{
    Unload();
}

/*
================================================================================

Image::Load

    Loads an image from the specified path.  If an image was already loaded,
    it will be unloaded and replaced using the new image.

================================================================================
*/
bool Image::Load(const std::string& path)
{
    Unload();

    mSurface = IMG_Load(path.c_str());

    if (!mSurface) {
        std::cerr << "*** Failed to load image '" << path << "': " << IMG_GetError() << std::endl;
    }

    return mSurface != NULL;
}

/*
================================================================================

Image::Alloc

    Allocates memory for a "blank" image.  If an image was already loaded,
    it will be unloaded and replaced using the new image.

    After allocation, the image contents (the pixel color buffer) are not
    initialized in any way.  Clients are responsible for filling the pixel buffer
    with meaningful color values.  Clients can gain access to the pixel buffer
    using the methods GetData and GetRow.  Clients must ensure that they don't
    write outside of the bounds of the pixel buffer.

================================================================================
*/
bool Image::Alloc(int width, int height, int bytesPerPixel)
{
    Unload();

    if (bytesPerPixel < 1 || bytesPerPixel > 4) {
        std::cerr << "*** Invalid bytesPerPixel for image" << std::endl;
        return false;
    }

    int depth = bytesPerPixel * 8;      // bits per pixel

    mSurface = SDL_CreateRGBSurface(0, width, height, depth, 0, 0, 0, 0);

    return mSurface != NULL;
}

/*
================================================================================

Image::Unload

    Unloads the image if one is loaded.

================================================================================
*/
void Image::Unload()
{
    if (mSurface) {
        SDL_FreeSurface(mSurface);
        mSurface = NULL;
    }
}


/*
================================================================================

Texture constructor

    This constructor can be used for single-cell textures, i.e., textures
    that don't contain multiple animation cells or sub-images.

================================================================================
*/
Texture::Texture(const std::string& name, SDL_Texture* tex)
    : mName(name)
    , mTex(tex)
    , mWidth(0)
    , mHeight(0)
    , mNumCells(0)
    , mCellWidth(0)
    , mCellHeight(0)
{
    // query texture size to set member variables
    if (tex) {
        SDL_QueryTexture(tex, NULL, NULL, &mWidth, &mHeight);

        // this constructor assumes that there's only once cell (entire texture)
        mNumCells = 1;

        mCellWidth = mWidth;
        mCellHeight = mHeight;
    }
}

/*
================================================================================

Texture constructor

    This constructor can be used for multi-cell textures, i.e., textures
    that contain multiple animation cells or sub-images.  The cells are assumed
    to be arranged in a single horizontal row.

================================================================================
*/
Texture::Texture(const std::string& name, SDL_Texture* tex, int numCells)
    : mName(name)
    , mTex(tex)
    , mWidth(0)
    , mHeight(0)
    , mNumCells(0)
    , mCellWidth(0)
    , mCellHeight(0)
{
    // query texture size to properly set member variables
    if (tex) {
        SDL_QueryTexture(tex, NULL, NULL, &mWidth, &mHeight);

        mNumCells = numCells;

        mCellWidth = mWidth / numCells;
        mCellHeight = mHeight;
    }
}

/*
================================================================================

Texture destructor

    Deletes the owned texture, if any.

================================================================================
*/
Texture::~Texture()
{
    // delete the SDL_Texture
    if (mTex) {
        SDL_DestroyTexture(mTex);
    }
}


/*
================================================================================

TextureManager constructor

    Initializes everything to known invalid values.  Any meaningful initialization
    is deferred to the Initialize method.

================================================================================
*/
TextureManager::TextureManager()
    : mRenderer(NULL)
    , mRootDir()
    , mDefaultTex(NULL)
{
}

/*
================================================================================

TextureManager destructor

    Deletes all client textures as well as the default texture.

================================================================================
*/
TextureManager::~TextureManager()
{
    // delete all client textures
    DeleteAll();

    // delete the default texture
    delete mDefaultTex;
}

/*
================================================================================

TextureManager::Initialize

    Performs proper initialization, including setting up the texture root
    directory path and creating the default texture.

    Typically, this method should be called only once, before calling any
    methods for loading or getting textures.

================================================================================
*/
bool TextureManager::Initialize(SDL_Renderer* renderer, const std::string& rootDir)
{
    if (!renderer) {
        return false;
    }

    mRenderer = renderer;
    mRootDir = rootDir;

    // make sure rootDir ends with dir separator, so that relative paths can be easily appended
    if (!mRootDir.empty()) {
        char last = mRootDir[mRootDir.length() - 1];
        if (last != '/' && last != '\\') {
            mRootDir += '/';
        }
    }

	// Load the font
	font = TTF_OpenFont("fonts/FreeSerifBold.ttf", 24);
	if (font == NULL)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
		return NULL;
	}

    if (mDefaultTex) {
        DeleteTexture(mDefaultTex);
    }

    mDefaultTex = CreateDefaultTexture();

    if (mDefaultTex) {
        return true;
    } else {
        return false;
    }
}

/*
================================================================================

TextureManager::CreateDefaultTexture

    A private method that procedurally generates the image to be used as the
    default texture.  It generates a coloured checkerboard, which should be
    easily spotted if it ever gets rendered.  If you see the default texture 
    being rendered, it probably means that a texture name lookup failed, so
    a client is trying to use an incorrect texture name or a texture with that
    name has not been loaded during game initialization.

    The storage for the default texture image gets allocated using an Image
    object.  We then move through the pixel buffer one row at a time, setting
    the RGB values for each pixel.  To create the two-colored checkerboard,
    we determine which row and column of the checkerboard we're in.  If the
    row and column numbers are both even or both odd, we use one color.  If
    one is even and the other is odd, we use the second color.

    This method returns a pointer to the new Texture object that contains the
    default texture.  This texture is not added to the name lookup table,
    which is only used for client textures.

================================================================================
*/
Texture* TextureManager::CreateDefaultTexture()
{
    int width = 64;
    int height = 64;
    int checkerSize = 8;

    Image img;
    if (!img.Alloc(width, height, 3)) {
        return NULL;
    }

    for (int y = 0; y < height; y++) {
        int row = y / checkerSize;
        Uint8* p = img.GetRow(y);
        for (int x = 0; x < width; x++) {
            int col = x / checkerSize;
            if ((row & 1) ^ (col & 1)) {
                *p++ = 224;
                *p++ = 0;
                *p++ = 224;
            } else {
                *p++ = 64;
                *p++ = 64;
                *p++ = 64;
            }
        }
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, img.GetSurface());
    if (!tex) {
        std::cerr << "*** Failed to create default texture" << SDL_GetError() << std::endl;
        return NULL;
    }

    // create the texture object, but don't add it to the lookup table - it's special :)
    return new Texture("_Default", tex);
}

/*
================================================================================

TextureManager::LoadTexture

    Loads a texture from a file with the specified filename.  The filename is
    assumed to be relative to the root texture directory path.  Setting the
    root texture directory path during initialization eliminates the need to
    specify potentially long paths for every single texture to be loaded.
    Instead of asking to load

      "assets/graphics/textures/foo.tga"
      "assets/graphics/textures/bar.png"
      "assets/graphics/textures/bazinga.png"

    we can initialize the root texture directory to "assets/graphics/textures/"
    and then just ask to load "foo.tga", "bar.png", and "bazinga.png".

    This function also requires the client to specify the number of cells
    the texture contains, which defaults to 1.

    The name argument must be a unique identifier for this texture.  The
    texture gets added to the lookup table using this name as the key, so
    other clients can retrieve the texture later using its name.  If a name
    already exists in the lookup table, requests to load another texture
    under the same name will fail.

    This method loads the image from the specified path, then delegates the
    actual Texture creation to the method that loads textures from Images.

================================================================================
*/
Texture* TextureManager::LoadTexture(const std::string& name, const std::string& filename, int numCells)
{
    // determine full path
    std::string path = mRootDir + filename;

    // load the image
    Image img(path);                      

    // create texture from image
    return LoadTexture(name, img, numCells);
}

/*
================================================================================

TextureManager::LoadTexture

    Loads a texture from an Image, which contains the raw pixel data.

    Refer to the overload of this method for other details.

================================================================================
*/
Texture* TextureManager::LoadTexture(const std::string& name, const Image& img, int numCells)
{
    if (img.IsLoaded()) {

        // first, check if the name already exists in our lookup table
        if (mTextures.find(name) != mTextures.end()) {
            std::cerr << "*** Texture with name '" << name << "' already exists" << std::endl;
            return NULL;
        }

        SDL_Surface* surf = img.GetSurface();

        SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        if (!tex) {
            std::cerr << "*** Failed to create texture '" << name << "': " << SDL_GetError() << std::endl;
            return NULL;
        }

        // create a new Texture object
        Texture* texObj = new Texture(name, tex, numCells);

        // add it to our lookup table
        mTextures[name] = texObj;

        return texObj;

    } else {
        // invalid image, fail
        return NULL;
    }
}

/*
================================================================================

TextureManager::LoadTexture

    Loads a texture that represents a label specified by a string

    Refer to the overload of this method for other details.

================================================================================
*/
Texture* TextureManager::LoadTexture(const std::string& name, const char* text, SDL_Color text_color)
{
	// first, check if the name already exists in our lookup table
    if (mTextures.find(name) != mTextures.end())
	{
        std::cerr << "*** Texture with name '" << name << "' already exists" << std::endl;
        return NULL;
    }
	
	// Write the given text to a surface
	SDL_Surface *textSurface;
	textSurface = TTF_RenderText_Solid(font, text, text_color);

	if (textSurface == NULL)
	{
		std::cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << std::endl;
		return NULL;
	}

	// Convert the surface into an SDL texture
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    if (!tex) {
        std::cerr << "*** Failed to create texture '" << name << "': " << SDL_GetError() << std::endl;
        return NULL;
    }

    // create a new Texture object
    Texture* texObj = new Texture(name, tex, 1);

    // add it to our lookup table
    mTextures[name] = texObj;

    return texObj;
}

/*
================================================================================

TextureManager::GetTexture

    Returns a pointer to the Texture with the specified name.

    If the name is not found in the lookup table, a pointer to the default
    texture is returned.  FTW.

================================================================================
*/
Texture* TextureManager::GetTexture(const std::string& name) const
{
    std::map<std::string, Texture*>::const_iterator it = mTextures.find(name);
    if (it != mTextures.end()) {
        return it->second;
    } else {
        std::cerr << "*** Oops, texture '" << name << "' not found" << std::endl;
        return mDefaultTex; // return the default texture instead of a NULL pointer :)
    }
}

/*
================================================================================

TextureManager::DeleteTexture

    Deletes the Texture with the specified name.

    Clients can't delete Textures directly, since the TextureManager owns all
    the Textures it creates.  This method allows clients to ask the 
    TextureManager to delete a texture that is no longer needed.  Caution
    must be taken to ensure that no other clients still hold pointers to
    the texture.

================================================================================
*/
void TextureManager::DeleteTexture(const std::string& name)
{
    std::map<std::string, Texture*>::iterator it = mTextures.find(name);

    if (it != mTextures.end()) {

        // delete the texture object
        Texture* tex = it->second;
        delete tex;

        // remove the entry from the lookup table
        mTextures.erase(it);

    } else {
        std::cerr << "*** Warning: Can't delete texture '" << name << "': texture not in lookup table" << std::endl;
    }
}

/*
================================================================================

TextureManager::DeleteTexture

    Deletes the specified Texture object.

    This method delegates the actual deletion to the overload that takes the
    texture name as an argument.  Refer to that method for more details.

    Note that only textures known to have been loaded using this
    TextureManager should be deleted this way (this is only an issue
    if multiple TextureManagers are used in the same application).

================================================================================
*/
void TextureManager::DeleteTexture(Texture* tex)
{
    DeleteTexture(tex->GetName());
}

/*
================================================================================

TextureManager::DeleteAll

    Deletes all client textures.

    This method is a quick and sweet way to unload all textures without having
    to delete each texture independently.  This comes in handy at the end of
    the game or during major game state transitions (e.g., going from a UI menu
    to a gameplay state).

    Note that this method doesn't delete the default texture, which the 
    TextureManager keeps around until it gets shut down.

================================================================================
*/
void TextureManager::DeleteAll()
{
    // delete each texture object in the lookup table
    std::map<std::string, Texture*>::iterator it = mTextures.begin();
    for ( ; it != mTextures.end(); ++it) {
        Texture* tex = it->second;
        delete tex;
    }

    // clear the lookup table
    mTextures.clear();
}

} // end namespace
