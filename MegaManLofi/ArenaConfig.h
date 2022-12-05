#pragma once

#include <vector>

#include "ArenaTile.h"

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

      long long DefaultPlayerPositionX = 0;
      long long DefaultPlayerPositionY = 0;
   };
}
