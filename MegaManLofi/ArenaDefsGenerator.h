#pragma once

#include <memory>
#include <map>

namespace MegaManLofi
{
   class ArenaDefs;
   class WorldDefs;

   class ArenaDefsGenerator
   {
   public:
      static std::map<int, std::shared_ptr<ArenaDefs>> GenerateArenaDefsMap( const std::shared_ptr<WorldDefs> worldDefs );
   };
}
