#pragma once

#include "IFrameRateProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IGameClock : public IFrameRateProvider
   {
   public:
      virtual void SetMinimumFrameRate( long long frameRate ) = 0;

      virtual void StartFrame() = 0;
      virtual void EndFrame() = 0;
   };
}
