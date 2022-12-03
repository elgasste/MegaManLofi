#pragma once

#include "ISleeper.h"

namespace MegaManLofi
{
   class SleeperWrapper : public ISleeper
   {
   public:
      void Sleep( long long nanoseconds ) override;
   };
}
