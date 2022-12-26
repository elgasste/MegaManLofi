#pragma once

#include <vector>

#include "ArenaTile.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class ArenaDefs
   {
   public:
      long long DefaultTileWidth = 0;
      long long DefaultTileHeight = 0;

      int DefaultHorizontalTiles = 0;
      int DefaultVerticalTiles = 0;

      long long ActiveRegionWidth = 0;
      long long ActiveRegionHeight = 0;

      std::vector<ArenaTile> DefaultTiles;

      Coordinate<long long> DefaultPlayerPosition = { 0, 0 };
   };
}