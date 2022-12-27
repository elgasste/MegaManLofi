#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IUniqueNumberGenerator
   {
   public:
      virtual int GetNext() = 0;
   };
}
