#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   class EntityDefs
   {
   public:
      int BulletEntityMetaId = 0;
      float BulletVelocity = 0;
      Rectangle<float> BulletHitBox;
   };
}
