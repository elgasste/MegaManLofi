#pragma once

#include <memory>

namespace MegaManLofi
{
   class EntityDefs;

   class EntityDefsGenerator
   {
   public:
      static std::shared_ptr<EntityDefs> GenerateEntityDefs();
   };
}
