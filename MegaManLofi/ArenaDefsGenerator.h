#pragma once

#include <memory>

namespace MegaManLofi
{
   class ArenaDefs;

   class ArenaDefsGenerator
   {
   public:
      static std::shared_ptr<ArenaDefs> GenerateArenaDefs();
   };
}
