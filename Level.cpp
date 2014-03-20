#include "Level.h"
#include "Game.h"

Grid* CreateRandomLevel(const std::string& tileTexName)
{
    Game* game = Game::GetInstance();

    // get the tile texture
    GG::Texture* tex = game->GetTextureManager()->GetTexture(tileTexName);

    // how many different tiles are available in the texture?
    int numTiles = tex->GetNumCells();

    int tileWidth = tex->GetCellWidth();
    int tileHeight = tex->GetCellHeight();

    int scrWidth = game->GetScrWidth();
    int scrHeight = game->GetScrHeight();

    // world grid size based on screen size and tile size
    int numCols = scrWidth / tileWidth;
    int numRows = scrHeight / tileHeight;

    Grid* grid = new Grid;
    grid->Allocate(numCols, numRows, tileWidth, tileHeight);

    // generate a random arrangement of tiles (and blanks) in the grid
        for (int x = 0; x < numCols; x++) {

            Tile* tile = grid->GetTile(numRows-1, x);

            int r = GG::RandomInt(numTiles);

            tile->SetRenderable(new GG::Renderable(tex, r));
        }

    return grid;
}
