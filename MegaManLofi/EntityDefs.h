#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   class EntityDefs
   {
   public:
      long long BulletVelocity = 0;
      Rectangle<long long> BulletHitBox;
   };
}
