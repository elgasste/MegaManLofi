#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   class EntityDefs
   {
   public:
      int BulletEntityMetaId = 0;
      long long BulletVelocity = 0;
      Rectangle<long long> BulletHitBox;
   };
}
