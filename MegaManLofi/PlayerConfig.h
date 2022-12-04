#pragma once

#include "Rectangle.h"
#include "Direction.h"

namespace MegaManLofi
{
   class PlayerConfig
   {
   public:
      Rectangle DefaultHitBox = { 0., 0., 0., 0. };

      double DefaultVelocityX = 0.;
      double DefaultVelocityY = 0.;

      Direction DefaultDirection = (Direction)0;
   };
}
