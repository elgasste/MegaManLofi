#pragma once

#include <memory>

namespace MegaManLofi
{
   class WorldDefs;

   class WorldDefsGenerator
   {
   public:
      static std::shared_ptr<WorldDefs> GenerateWorldDefs();
   };
}
