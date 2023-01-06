#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   struct ItemInfo
   {
      Rectangle<float> HitBox;

      float MaxGravityVelocity = 0;
      float GravityAccelerationPerSecond = 0;
   };
}
