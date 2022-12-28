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
      virtual const Coordinate<long long>& GetArenaPosition() const = 0;
      virtual long long GetArenaPositionLeft() const = 0;
      virtual long long GetArenaPositionTop() const = 0;
      virtual long long GetVelocityX() const = 0;
      virtual long long GetVelocityY() const = 0;
      virtual Direction GetDirection() const = 0;
      virtual const Rectangle<long long>& GetHitBox() const = 0;
      virtual MovementType GetMovementType() const = 0;
   };
}
