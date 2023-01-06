#pragma once

#include <vector>

#include "ArenaTile.h"
#include "Coordinate.h"
#include "SpawnPoint.h"

namespace MegaManLofi
{
   class ArenaDefs
   {
   public:
      int ArenaId = 0;

      int HorizontalTiles = 0;
      int VerticalTiles = 0;

      std::vector<ArenaTile> Tiles;

      Coordinate<float> PlayerStartPosition = { 0, 0 };

      std::vector<SpawnPoint> SpawnPoints;
   };
}
