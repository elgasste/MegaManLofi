#pragma once

#include <memory>
#include <map>

namespace MegaManLofi
{
   class ArenaDefs;

   class StageDefs
   {
   public:

      int StartArenaId = 0;

      std::map<int, std::shared_ptr<ArenaDefs>> ArenaMap;
   };
}
