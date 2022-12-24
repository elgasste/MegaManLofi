#pragma once

#include <memory>

namespace MegaManLofi
{
   class PlayerDefs;

   class PlayerDefsGenerator
   {
   public:
      static std::shared_ptr<PlayerDefs> GeneratePlayerDefs();
   };
}
