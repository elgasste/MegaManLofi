#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   struct EnemyInfo
   {
      Rectangle<float> HitBox;

      float MaxGravityVelocity = 0;
      float GravityAccelerationPerSecond = 0;

      int MaxHealth = 0;
      float DamageInvulnerabilitySeconds = 0;
      float KnockBackSeconds = 0;
      float KnockBackVelocity = 0;
   };
}
