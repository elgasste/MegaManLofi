#pragma once

#include <memory>

namespace MegaManLofi
{
   class PlayerDefs;
   class UniqueNumberGenerator;

   class PlayerDefsGenerator
   {
   public:
      static std::shared_ptr<PlayerDefs> GeneratePlayerDefs( const std::shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator );
   };
}
