#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   struct ProjectileInfo
   {
      Rectangle<float> HitBox;
      float Velocity;
   };
}
