#pragma once

#include "IFrameRateProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IGameClock : public IFrameRateProvider
   {
   public:
      virtual void StartFrame() = 0;
      virtual void WaitForNextFrame() = 0;

      virtual long long GetTotalFrameCount() const = 0;
      virtual long long GetLagFrameCount() const = 0;
   };
}
