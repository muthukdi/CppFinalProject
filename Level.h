#ifndef LEVEL_H_
#define LEVEL_H_

#include "Grid.h"


Grid* CreateRandomLevel(const std::string& tileTexName);

Grid* LoadLevel(const std::string& filename, bool items);

#endif
