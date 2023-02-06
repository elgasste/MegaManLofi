#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ReadOnlyEntity.h>

class mock_ReadOnlyEntity : public MegaManLofi::ReadOnlyEntity
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
};
