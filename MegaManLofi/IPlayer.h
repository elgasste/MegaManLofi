#pragma once

#include "IPlayerInfoProvider.h"
#include "Rectangle.h"

namespace MegaManLofi
{
   enum class Direction;

   class __declspec( novtable ) IPlayer : public IPlayerInfoProvider
   {
   public:
      virtual const Rectangle& GetHitBox() const = 0;

      virtual double GetVelocityX() const = 0;
      virtual double GetVelocityY() const = 0;

      virtual void Push( Direction direction ) = 0;
      virtual void Point( Direction direction ) = 0;
      virtual void Jump() = 0;
      virtual void ApplyFriction() = 0;
      virtual void ApplyGravity() = 0;
      virtual void StopX() = 0;
      virtual void StopY() = 0;
   };
}
