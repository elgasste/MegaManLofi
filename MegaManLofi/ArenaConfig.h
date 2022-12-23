#pragma once

#include <vector>

#include "ArenaTile.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class ArenaConfig
   {
   public:
      long long DefaultTileWidth = 0;
      long long DefaultTileHeight = 0;

      int DefaultHorizontalTiles = 0;
      int DefaultVerticalTiles = 0;

      std::vector<ArenaTile> DefaultTiles;

      Coordinate<long long> DefaultPlayerPosition = { 0, 0 };
   };
}
