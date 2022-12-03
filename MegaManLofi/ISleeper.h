#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) ISleeper
   {
   public:
      virtual void Sleep( long long nanoseconds ) = 0;
   };
}
