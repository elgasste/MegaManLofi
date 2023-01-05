#pragma once

#include <memory>
#include <map>
#include <vector>

#include "ArenaPortal.h"
#include "Direction.h"

namespace MegaManLofi
{
   class ArenaDefs;

   class StageDefs
   {
   public:

      int StartArenaId = 0;

      std::map<int, std::shared_ptr<ArenaDefs>> ArenaMap;
      std::map<Direction, std::vector<ArenaPortal>> ArenaPortalMap;
   };
}
