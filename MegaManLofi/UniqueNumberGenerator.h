#pragma once

namespace MegaManLofi
{
   class UniqueNumberGenerator
   {
   public:
      UniqueNumberGenerator();

      virtual int GetNext();

   private:
      int _nextNumber;
   };
}
