#pragma once

#include "IFrameRateProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IGameClock : public IFrameRateProvider
   {
   public:
      virtual void StartFrame() = 0;
      virtual void EndFrame() = 0;

      virtual long long GetElapsedNanoseconds() const = 0;
      virtual long long GetAverageFrameRate() const = 0;
   };
}
