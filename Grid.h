#ifndef GRID_H_
#define GRID_H_

#include "GG_Renderable.h"

#include <vector>

class Tile {
    GG::Renderable*         mRenderable;

public:
                            Tile();
                            ~Tile();

    void                    SetRenderable(GG::Renderable* renderable);  // takes ownership

    GG::Renderable*         GetRenderable()		   { return mRenderable; }

	void					SetGrayscale(bool grayscale);
};


class Grid {
    std::vector<std::vector<Tile>> mTiles;

    int                     mNumCols;
    int                     mNumRows;

    int                     mTileWidth;
    int                     mTileHeight;

    Tile                    mHedgeTile; // special tile that represents the outer boundary of the grid

public:
                            Grid();
                            ~Grid();

    void                    Allocate(int numCols, int numRows, int tileWidth, int tileHeight);

    int                     NumRows() const     { return mNumRows; }
    int                     NumCols() const     { return mNumCols; }

    int                     TileWidth() const   { return mTileWidth; }
    int                     TileHeight() const  { return mTileHeight; }

    Tile*                   GetTile(int row, int col);
    const Tile*             GetTile(int row, int col) const;

    bool                    IsHedgeTile(const Tile* tile) const    { return tile == &mHedgeTile; }
};


inline Tile* Grid::GetTile(int row, int col)
{
    if (row >= 0 && row < mNumRows && col >= 0 && col < mNumCols) {
        return &mTiles[row][col];
    } else {
        //return NULL;
        return &mHedgeTile; // avoid returning NULL pointer
    }
}

inline const Tile* Grid::GetTile(int row, int col) const
{
    if (row >= 0 && row < mNumRows && col >= 0 && col < mNumCols) {
        return &mTiles[row][col];
    } else {
        //return NULL;
        return &mHedgeTile; // avoid returning NULL pointer
    }
}

#endif
