#pragma once

#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   struct SpawnPoint
   {
      int EntityMetaId = 0;
      Coordinate<float> ArenaPosition;
      Direction Direction;
      bool IsActive = false;
      bool HasSpawned = false;
      bool ReSpawns = false;
      bool ReSpawnsAtInterval = false;
      float ReSpawnIntervalSeconds = 0;
      float IntervalElapsedSeconds = 0;
   };
}
