#pragma once

#include <memory>

namespace MegaManLofi
{
   class FrameRateDefs;

   class FrameRateDefsGenerator
   {
   public:
      static std::shared_ptr<FrameRateDefs> GenerateFrateRateDefs();
   };
}
