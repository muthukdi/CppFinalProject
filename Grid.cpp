#include "GG_Common.h"

#include "Grid.h"
#include "Game.h"

Tile::Tile()
    : mRenderable(NULL)
{
}

Tile::~Tile()
{
    delete mRenderable;  // we own it
}

void Tile::SetRenderable(GG::Renderable* renderable)
{
    delete mRenderable;         // delete old one, if any
    mRenderable = renderable;   // set new one 
}

void Tile::SetGrayscale(bool grayscale)
{
	mRenderable->SetGrayscale(grayscale);
}


Grid::Grid()
    : mNumCols(0)
    , mNumRows(0)
    , mTileWidth(0)
    , mTileHeight(0)
{
}

void Grid::Allocate(int numCols, int numRows, int tileWidth, int tileHeight)
{
    mTiles.clear();  // clear any old contents

    mTiles.resize(numRows);
    for (int i = 0; i < numRows; i++) {
        mTiles[i].resize(numCols);
    }

    mNumCols = numCols;
    mNumRows = numRows;
    mTileWidth = tileWidth;
    mTileHeight = tileHeight;
}

Grid::~Grid()
{
    // nothing to do
}
