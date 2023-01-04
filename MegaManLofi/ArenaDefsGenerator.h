#pragma once

#include <memory>

namespace MegaManLofi
{
   class ArenaDefs;
   class WorldDefs;

   class ArenaDefsGenerator
   {
   public:
      static std::shared_ptr<ArenaDefs> GenerateArenaDefs( const std::shared_ptr<WorldDefs> worldDefs );
   };
}
