#pragma once

#include <map>

namespace MegaManLofi
{
   class ArenaDefs;

   class StageDefs
   {
   public:

      int StartArenaId = 0;

      std::map<int, ArenaDefs> ArenaMap;
   };
}
