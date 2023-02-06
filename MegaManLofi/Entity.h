#pragma once

#include <memory>

#include "ReadOnlyEntity.h"
#include "EntityCollisionPayload.h"

namespace MegaManLofi
{
   class IBehavior;

   class Entity : public ReadOnlyEntity
   {
   public:
      Entity();

      virtual void SetUniqueId( int id ) { _uniqueId = id; }
      virtual void SetEntityType( EntityType type ) { _entityType = type; }
      virtual void SetEntityMetaId( int id ) { _entityMetaId = id; }
      virtual void SetBehavior( const std::shared_ptr<IBehavior> behavior );
      virtual void SetArenaPosition( Coordinate<float> position ) { _arenaPosition = position; }
      virtual void SetVelocityX( float velocity ) { _velocityX = velocity; }
      virtual void SetVelocityY( float velocity ) { _velocityY = velocity; }
      virtual void SetDirection( Direction direction ) { _direction = direction; }
      virtual void SetHitBox( Rectangle<float> hitBox ) { _hitBox = hitBox; }
      virtual void SetMovementType( MovementType type ) { _movementType = type; }
      virtual void SetMaxGravityVelocity( float velocity ) { _maxGravityVelocity = velocity; }
      virtual void SetGravityAccelerationPerSecond( float acceleration ) { _gravityAccelerationPerSecond = acceleration; }
      virtual void SetFrictionDecelerationPerSecond( float deceleration ) { _frictionDecelerationPerSecond = deceleration; }
      virtual void SetHealth( unsigned int health ) { _health = health; }
      virtual void SetDamageInvulnerabilitySeconds( float seconds ) { _damageInvulnerabilitySeconds = seconds; }

      virtual void Act();
      virtual void StopX() { _velocityX = 0; }
      virtual void StopY() { _velocityY = 0; }
      virtual bool TakeCollisionPayload( const EntityCollisionPayload& payload ) { return false; }

   private:
      std::shared_ptr<IBehavior> _behavior;
   };
}
