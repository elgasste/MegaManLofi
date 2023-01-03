#pragma once

#include "EntityType.h"
#include "Coordinate.h"
#include "Rectangle.h"
#include "Direction.h"
#include "MovementType.h"

namespace MegaManLofi
{
   class ReadOnlyEntity
   {
   public:
      ReadOnlyEntity();

      virtual int GetUniqueId() const { return _uniqueId; }
      virtual EntityType GetEntityType() const { return _entityType; }
      virtual int GetEntityMetaId() const { return _entityMetaId; }
      virtual const Coordinate<float>& GetArenaPosition() const { return _arenaPosition; }
      virtual float GetArenaPositionLeft() const { return _arenaPosition.Left; }
      virtual float GetArenaPositionTop() const { return _arenaPosition.Top; }
      virtual float GetVelocityX() const { return _velocityX; }
      virtual float GetVelocityY() const { return _velocityY; }
      virtual Direction GetDirection() const { return _direction; }
      virtual const Rectangle<float>& GetHitBox() const { return _hitBox; }
      virtual MovementType GetMovementType() const { return _movementType; }

   protected:
      int _uniqueId;
      EntityType _entityType;
      int _entityMetaId;
      Coordinate<float> _arenaPosition;
      float _velocityX;
      float _velocityY;
      Direction _direction;
      Rectangle<float> _hitBox;
      MovementType _movementType;
   };
}
