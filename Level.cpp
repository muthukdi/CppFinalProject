#include "Level.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Crawler.h"
#include "CrawlerWeak.h"
#include "CrawlerStrong.h"
#include "Coin.h"

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

	for (int y = 0; y < numRows; y++) {
        for (int x = 0; x < numCols; x++) {

            Tile* tile = grid->GetTile(y, x);

            int r = GG::RandomInt(3 * numTiles);

            if (r < numTiles) {
                tile->SetRenderable(new GG::Renderable(tex, r));  // tile takes ownership of renderable
            } else {
                tile->SetRenderable(NULL);
            }
        }
    }

    return grid;
}

Grid* LoadLevel(const std::string& filename, bool items)
{
	std::fstream f(filename);
	//f.open(filename);
	if (!f.good())
	{
		std::cerr << "*** Error: failed to open " << filename << std::endl;
		return NULL;
	}

	std::string line;
	std::vector<std::string> lines;

	while (std::getline(f, line))
	{
		lines.push_back(line);
	}

	// Just outputs the file contents (for debugging)
	for (unsigned i = 0; i < lines.size(); i++)
	{
		std::cout << lines[i] << std::endl;
	}

	unsigned numRows = lines.size();
	unsigned numCols = 0;
	if (numRows > 0)
	{
		numCols = lines[0].length();
		for (unsigned i = 1; i < lines.size(); i++)
		{
			if (lines[i].length() != numCols)
			{
				std::cerr << "*** Error: Inconsistent number of lines" << std::endl;
				return NULL;
			}
		}
	}

	Game* game = Game::GetInstance();
	GG::Texture* tex = game->GetTextureManager()->GetTexture("Tiles");
	GG::Texture* tex2 = game->GetTextureManager()->GetTexture("Tiles2");

	int numCells = tex->GetNumCells();
	int tileWidth = tex->GetCellWidth();
    int tileHeight = tex->GetCellHeight();
	Grid* grid = new Grid;
	grid->Allocate(numCols, numRows, tileWidth, tileHeight);

	for (unsigned row = 0; row < numRows; row++)
	{
		for (unsigned col = 0; col < numCols; col++)
		{
			char c = lines[row][col];
			Tile* tile = grid->GetTile(row, col);
			switch (c)
			{
			case '.':
				break;
			case 'w':
			{
				if (items)
				{
					Crawler* crawler = new CrawlerWeak(col*tileWidth, (row+1)*tileHeight, true);
					crawler->SetDirection(GG::RandomSign());
					game->GetCrawlers()->push_back(crawler);
				}
				break;
			}
			case 's':
			{
				if (items)
				{
					Crawler* crawler = new CrawlerStrong(col*tileWidth, (row+1)*tileHeight, false);
					crawler->SetDirection(GG::RandomSign());
					game->GetCrawlers()->push_back(crawler);
				}
				break;
			}
			case 'c':
			{
				Coin* coin = new Coin(col*tileWidth, (row+1)*tileHeight);
				game->GetCoins()->push_back(coin);
				break;
			}
			case '@':
			{
				int r = GG::RandomInt(numCells);
				tile->SetRenderable(new GG::Renderable(tex2, r));
				break;
			}
			case '#':
			{
				int r = GG::RandomInt(numCells);
				tile->SetRenderable(new GG::Renderable(tex, r));
				break;
			}
			default:
				std::cerr << "Don't know what to do with character " << c << std::endl;
				break;
			}
		}
	}
	return grid;
}