#pragma once

#include "IRandom.h"

namespace MegaManLofi
{
   class RandomWrapper : public IRandom
   {
   public:
      int GetInt( int min, int max ) const override;
   };
}
