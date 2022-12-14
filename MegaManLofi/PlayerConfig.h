#pragma once

#include "Rectangle.h"
#include "Direction.h"

namespace MegaManLofi
{
   class PlayerConfig
   {
   public:
      long long DefaultVelocityX = 0;
      long long DefaultVelocityY = 0;

      unsigned int DefaultLives = 0;
      Direction DefaultDirection = (Direction)0;
      Rectangle<long long> DefaultHitBox = { 0, 0, 0, 0 };
   };
}
