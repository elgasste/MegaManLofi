#pragma once

#include "Rectangle.h"
#include "Direction.h"

namespace MegaManLofi
{
   class PlayerConfig
   {
   public:
      Rectangle HitBox = { 0., 0., 0., 0. };

      double StartVelocityX = 0.;
      double StartVelocityY = 0.;

      double MaxPushVelocity = 0.;
      double MaxGravityVelocity = 0.;

      double PushAccelerationPerSecond = 0.;
      double FrictionDecelerationPerSecond = 0.;
      double GravityAccelerationPerSecond = 0.;

      Direction StartDirection = (Direction)0;
   };
}
