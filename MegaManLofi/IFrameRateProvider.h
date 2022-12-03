#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IFrameRateProvider
   {
   public:
      virtual int GetFramesPerSecond() const = 0;
   };
}
