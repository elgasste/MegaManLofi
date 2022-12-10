#pragma once

#include "IRandom.h"

namespace MegaManLofi
{
   class RandomWrapper : public IRandom
   {
   public:
      unsigned int GetUnsignedInt( unsigned int min, unsigned int max ) const override;
   };
}
