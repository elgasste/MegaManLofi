#pragma once

#include "IRandom.h"

namespace MegaManLofi
{
   class RandomWrapper : public IRandom
   {
   public:
      float GetFloat( float min, float max ) const override;
      int GetInt( int min, int max ) const override;
   };
}
