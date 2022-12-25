#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IFrameRateProvider
   {
   public:
      virtual long long GetCurrentFrame() const = 0;
      virtual double GetFrameSeconds() const = 0;
   };
}
