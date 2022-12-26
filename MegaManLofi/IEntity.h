#pragma once

#include "Coordinate.h"
#include "Rectangle.h"
#include "Direction.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IEntity
   {
   public:
      virtual const Coordinate<long long>& GetArenaPosition() const = 0;
      virtual long long GetArenaPositionLeft() const = 0;
      virtual long long GetArenaPositionTop() const = 0;

      virtual long long GetVelocityX() const = 0;
      virtual long long GetVelocityY() const = 0;
      virtual Direction GetDirection() const = 0;
      virtual const Rectangle<long long>& GetHitBox() const = 0;

      virtual bool IsMoving() const = 0;
   };
}