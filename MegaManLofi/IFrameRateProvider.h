#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IFrameRateProvider
   {
   public:
      virtual unsigned int GetFramesPerSecond() const = 0;
      virtual long long GetCurrentFrame() const = 0;
      virtual double GetSecondsPerFrame() const = 0;
   };
}
