#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IFrameRateProvider
   {
   public:
      virtual long long GetMinimumFrameRate() const = 0;
      virtual bool HasMinimumFrameRate() const = 0;
      virtual long long GetCurrentFrame() const = 0;
      virtual double GetFrameSeconds() const = 0;
      virtual long long GetLagFrameCount() const = 0;
   };
}
