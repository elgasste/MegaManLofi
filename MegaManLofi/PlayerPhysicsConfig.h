#pragma once

namespace MegaManLofi
{
   class PlayerPhysicsConfig
   {
   public:
      double MaxPushVelocity = 0.;
      double MaxGravityVelocity = 0.;

      double PushAccelerationPerSecond = 0.;
      double FrictionDecelerationPerSecond = 0.;
      double GravityAccelerationPerSecond = 0.;
   };
}
