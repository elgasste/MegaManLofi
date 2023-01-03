#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IFrameRateProvider
   {
   public:
      virtual long long GetMinimumFrameRate() const = 0;
      virtual bool HasMinimumFrameRate() const = 0;
      virtual long long GetCurrentFrame() const = 0;
      virtual float GetFrameSeconds() const = 0;
      virtual long long GetLagFrameCount() const = 0;
      virtual long long GetElapsedNanoseconds() const = 0;
      virtual long long GetAverageFrameRate() const = 0;
   };
}
