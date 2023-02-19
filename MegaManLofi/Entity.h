#pragma once

#include <memory>

#include "ReadOnlyEntity.h"
#include "EntityCollisionPayload.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IBehavior;

   class Entity : public ReadOnlyEntity
   {
   public:
      Entity() { }
      Entity( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      virtual void SetUniqueId( int id ) { _uniqueId = id; }
      virtual void SetEntityType( EntityType type ) { _entityType = type; }
      virtual void SetEntityMetaId( int id ) { _entityMetaId = id; }
      virtual void SetBehavior( const std::shared_ptr<IBehavior> behavior );
      virtual void SetArenaPosition( const Coordinate<float>& position ) { _arenaPosition = position; }
      virtual void SetVelocityX( float velocity ) { _velocityX = velocity; }
      virtual void SetVelocityY( float velocity ) { _velocityY = velocity; }
      virtual void SetDirection( Direction direction ) { _direction = direction; }
      virtual void SetHitBox( const Rectangle<float>& hitBox ) { _hitBox = hitBox; }
      virtual void SetMovementType( MovementType type ) { _movementType = type; }
      virtual void SetMaxGravityVelocity( float velocity ) { _maxGravityVelocity = velocity; }
      virtual void SetGravityAccelerationPerSecond( float acceleration ) { _gravityAccelerationPerSecond = acceleration; }
      virtual void SetFrictionDecelerationPerSecond( float deceleration ) { _frictionDecelerationPerSecond = deceleration; }
      virtual void SetHealth( unsigned int health ) { _health = health; }
      virtual void SetMaxHealth( unsigned int health ) { _maxHealth = health; }
      virtual void SetDamageInvulnerabilitySeconds( float seconds ) { _damageInvulnerabilitySeconds = seconds; }
      virtual void SetKnockBackSeconds( float seconds ) { _knockBackSeconds = seconds; }
      virtual void SetKnockBackVelocity( float velocity ) { _knockBackVelocity = velocity; }

      virtual void Tick();
      virtual void StopX() { _velocityX = 0; }
      virtual void StopY() { _velocityY = 0; }
      virtual bool TakeCollisionPayload( const EntityCollisionPayload& payload, float giverVelocityX );

   protected:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

   private:
      void CheckKnockBack( float giverVelocityX );

   private:
      std::shared_ptr<IBehavior> _behavior;
   };
}
