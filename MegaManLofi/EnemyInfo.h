#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   struct EnemyInfo
   {
      Rectangle<float> HitBox;

      float MaxGravityVelocity = 0;
      float GravityAccelerationPerSecond = 0;
   };
}
