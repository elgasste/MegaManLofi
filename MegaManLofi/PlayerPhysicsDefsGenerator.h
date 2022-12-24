#pragma once

#include <memory>

namespace MegaManLofi
{
   class PlayerPhysicsDefs;

   class PlayerPhysicsDefsGenerator
   {
   public:
      static std::shared_ptr<PlayerPhysicsDefs> GeneratePlayerPhysicsDefs();
   };
}
