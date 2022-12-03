#pragma once

#include "IHighResolutionClock.h"

namespace MegaManLofi
{
   class HighResolutionClockWrapper : public IHighResolutionClock
   {
   public:
      HighResolutionClockWrapper();
      ~HighResolutionClockWrapper();

      long long Now() override;
   };
}
