#pragma once

#include <memory>

namespace MegaManLofi
{
   class StageDefs;
   class ArenaDefs;
   class WorldDefs;

   class StageDefsGenerator
   {
   public:
      static std::shared_ptr<StageDefs> GenerateStageDefs( const std::shared_ptr<WorldDefs> worldDefs );
   };
}
