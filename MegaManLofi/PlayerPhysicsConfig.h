#pragma once

namespace MegaManLofi
{
   class PlayerPhysicsConfig
   {
   public:
      long long MaxPushVelocity = 0;
      long long MaxGravityVelocity = 0;

      long long PushAccelerationPerSecond = 0;
      long long FrictionDecelerationPerSecond = 0;
      long long JumpAccelerationPerSecond = 0;
      long long GravityAccelerationPerSecond = 0;
   };
}
