#pragma once

#include "IFrameRateProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IGameClock : public IFrameRateProvider
   {
   public:
      virtual long long GetMinimumFrameRate() const = 0;
      virtual void SetMinimumFrameRate( long long frameRate ) = 0;
      virtual bool HasMinimumFrameRate() const = 0;

      virtual void StartFrame() = 0;
      virtual void EndFrame() = 0;

      virtual long long GetElapsedNanoseconds() const = 0;
      virtual long long GetAverageFrameRate() const = 0;
   };
}
