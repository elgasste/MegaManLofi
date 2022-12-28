#pragma once

#include "IUniqueNumberGenerator.h"

namespace MegaManLofi
{
   class UniqueNumberGenerator : public IUniqueNumberGenerator
   {
   public:
      UniqueNumberGenerator();

      int GetNext() override;

   private:
      int _nextNumber;
   };
}
