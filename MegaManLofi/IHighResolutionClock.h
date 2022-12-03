#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IHighResolutionClock
   {
   public:
      virtual long long Now() = 0;
   };
}
