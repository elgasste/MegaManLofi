#pragma once

#include "Rectangle.h"
#include "Direction.h"
#include "MovementType.h"

namespace MegaManLofi
{
   class PlayerDefs
   {
   public:
      int DefaultUniqueId = 0;
      int DefaultEntityMetaId = 0;

      float DefaultVelocityX = 0;
      float DefaultVelocityY = 0;

      float MaxGravityVelocity = 0;
      float GravityAccelerationPerSecond = 0;

      unsigned int DefaultLives = 0;
      Direction DefaultDirection = (Direction)0;
      Rectangle<float> DefaultHitBox = { 0, 0, 0, 0 };
      MovementType DefaultMovementType = (MovementType)0;
   };
}
