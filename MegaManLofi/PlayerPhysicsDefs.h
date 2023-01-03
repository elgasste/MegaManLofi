#pragma once

namespace MegaManLofi
{
   class PlayerPhysicsDefs
   {
   public:
      float MaxPushVelocity = 0;
      float MaxGravityVelocity = 0;

      float PushAccelerationPerSecond = 0;
      float FrictionDecelerationPerSecond = 0;
      float JumpAccelerationPerSecond = 0;
      float GravityAccelerationPerSecond = 0;

      float MaxJumpExtensionSeconds = 0.;
   };
}
