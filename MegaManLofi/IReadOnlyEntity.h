#pragma once

#include "EntityType.h"
#include "Coordinate.h"
#include "Rectangle.h"
#include "Direction.h"
#include "MovementType.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IReadOnlyEntity
   {
   public:
      virtual int GetUniqueId() const = 0;
      virtual EntityType GetEntityType() const = 0;
      virtual int GetEntityMetaId() const = 0;
      virtual const Coordinate<float>& GetArenaPosition() const = 0;
      virtual float GetArenaPositionLeft() const = 0;
      virtual float GetArenaPositionTop() const = 0;
      virtual float GetVelocityX() const = 0;
      virtual float GetVelocityY() const = 0;
      virtual Direction GetDirection() const = 0;
      virtual const Rectangle<float>& GetHitBox() const = 0;
      virtual MovementType GetMovementType() const = 0;
   };
}
