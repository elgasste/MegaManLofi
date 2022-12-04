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

      double MaxPushVelocity = 0.;
      double MaxGravityVelocity = 0.;

      double PushAccelerationPerSecond = 0.;
      double FrictionDecelerationPerSecond = 0.;
      double GravityAccelerationPerSecond = 0.;
   };
}
