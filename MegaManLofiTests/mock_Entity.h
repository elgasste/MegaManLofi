#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/Entity.h>

class mock_Entity : public MegaManLofi::Entity
{
public:
   MOCK_METHOD( int, GetUniqueId, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::EntityType, GetEntityType, ( ), ( const, override ) );
   MOCK_METHOD( int, GetEntityMetaId, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Coordinate<float>&, GetArenaPosition, ( ), ( const, override ) );
   MOCK_METHOD( float, GetArenaPositionLeft, ( ), ( const, override ) );
   MOCK_METHOD( float, GetArenaPositionTop, ( ), ( const, override ) );
   MOCK_METHOD( float, GetVelocityX, ( ), ( const, override ) );
   MOCK_METHOD( float, GetVelocityY, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::Direction, GetDirection, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Rectangle<float>&, GetHitBox, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::MovementType, GetMovementType, ( ), ( const, override ) );
   MOCK_METHOD( float, GetMaxGravityVelocity, ( ), ( const, override ) );
   MOCK_METHOD( float, GetGravityAccelerationPerSecond, ( ), ( const, override ) );
   MOCK_METHOD( float, GetFrictionDecelerationPerSecond, ( ), ( const, override ) );
   MOCK_METHOD( unsigned int, GetHealth, ( ), ( const, override ) );
   MOCK_METHOD( unsigned int, GetMaxHealth, ( ), ( const, override ) );
   MOCK_METHOD( float, GetDamageInvulnerabilitySeconds, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsInvulnerable, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsKnockedBack, ( ), ( const, override ) );
   MOCK_METHOD( float, GetKnockBackSeconds, ( ), ( const, override ) );
   MOCK_METHOD( float, GetKnockBackVelocity, ( ), ( const, override ) );

   MOCK_METHOD( void, SetUniqueId, ( int ), ( override ) );
   MOCK_METHOD( void, SetEntityType, ( MegaManLofi::EntityType ), ( override ) );
   MOCK_METHOD( void, SetEntityMetaId, ( int ), ( override ) );
   MOCK_METHOD( void, SetBehavior, ( const std::shared_ptr<MegaManLofi::IBehavior> ), ( override ) );
   MOCK_METHOD( void, SetArenaPosition, ( const MegaManLofi::Coordinate<float>& ), ( override ) );
   MOCK_METHOD( void, SetVelocityX, ( float ), ( override ) );
   MOCK_METHOD( void, SetVelocityY, ( float ), ( override ) );
   MOCK_METHOD( void, SetDirection, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( void, SetHitBox, ( const MegaManLofi::Rectangle<float>& ), ( override ) );
   MOCK_METHOD( void, SetMovementType, ( MegaManLofi::MovementType ), ( override ) );
   MOCK_METHOD( void, SetMaxGravityVelocity, ( float ), ( override ) );
   MOCK_METHOD( void, SetGravityAccelerationPerSecond, ( float ), ( override ) );
   MOCK_METHOD( void, SetFrictionDecelerationPerSecond, ( float ), ( override ) );
   MOCK_METHOD( void, SetHealth, ( unsigned int ), ( override ) );
   MOCK_METHOD( void, SetMaxHealth, ( unsigned int ), ( override ) );
   MOCK_METHOD( void, SetDamageInvulnerabilitySeconds, ( float ), ( override ) );
   MOCK_METHOD( void, SetKnockBackSeconds, ( float ), ( override ) );
   MOCK_METHOD( void, SetKnockBackVelocity, ( float ), ( override ) );
   MOCK_METHOD( void, Tick, ( ), ( override ) );
   MOCK_METHOD( void, StopX, ( ), ( override ) );
   MOCK_METHOD( void, StopY, ( ), ( override ) );
   MOCK_METHOD( bool, TakeCollisionPayload, ( const MegaManLofi::EntityCollisionPayload& ), ( override ) );
};
