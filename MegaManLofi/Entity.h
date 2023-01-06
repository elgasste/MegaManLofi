#pragma once

#include "ReadOnlyEntity.h"

namespace MegaManLofi
{
   class Entity : public ReadOnlyEntity
   {
   public:
      virtual void SetUniqueId( int id ) { _uniqueId = id; }
      virtual void SetEntityType( EntityType type ) { _entityType = type; }
      virtual void SetEntityMetaId( int id ) { _entityMetaId = id; }
      virtual void SetArenaPosition( Coordinate<float> position ) { _arenaPosition = position; }
      virtual void SetVelocityX( float velocity ) { _velocityX = velocity; }
      virtual void SetVelocityY( float velocity ) { _velocityY = velocity; }
      virtual void SetDirection( Direction direction ) { _direction = direction; }
      virtual void SetHitBox( Rectangle<float> hitBox ) { _hitBox = hitBox; }
      virtual void SetMovementType( MovementType type ) { _movementType = type; }
      virtual void SetMaxGravityVelocity( float velocity ) { _maxGravityVelocity = velocity; }
      virtual void SetGravityAccelerationPerSecond( float acceleration ) { _gravityAccelerationPerSecond = acceleration; }

      virtual void StopX() { _velocityX = 0; }
      virtual void StopY() { _velocityY = 0; }
   };
}
