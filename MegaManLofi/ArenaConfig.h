#pragma once

#include <vector>

#include "ArenaTile.h"

namespace MegaManLofi
{
   class ArenaConfig
   {
   public:
      double DefaultTileWidth = 0.;
      double DefaultTileHeight = 0.;

      int DefaultHorizontalTiles = 0;
      int DefaultVerticalTiles = 0;

      std::vector<ArenaTile> DefaultTiles;

      double DefaultPlayerPositionX = 0.;
      double DefaultPlayerPositionY = 0.;
   };
}
