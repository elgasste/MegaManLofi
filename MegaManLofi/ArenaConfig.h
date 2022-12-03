#pragma once

#include <vector>

#include "ArenaTile.h"

namespace MegaManLofi
{
   class ArenaConfig
   {
   public:
      double TileWidth = 0.;
      double TileHeight = 0.;

      int HorizontalTiles = 0;
      int VerticalTiles = 0;

      std::vector<ArenaTile> Tiles;

      double PlayerStartX = 0.;
      double PlayerStartY = 0.;
   };
}
