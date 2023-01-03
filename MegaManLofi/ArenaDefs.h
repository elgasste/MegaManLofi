#pragma once

#include <vector>

#include "ArenaTile.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class ArenaDefs
   {
   public:
      float DefaultTileWidth = 0;
      float DefaultTileHeight = 0;

      int DefaultHorizontalTiles = 0;
      int DefaultVerticalTiles = 0;

      float ActiveRegionWidth = 0;
      float ActiveRegionHeight = 0;

      std::vector<ArenaTile> DefaultTiles;

      Coordinate<float> DefaultPlayerPosition = { 0, 0 };
   };
}
