#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntity.h>

class mock_Entity : public MegaManLofi::IEntity
{
public:
   MOCK_METHOD( int, GetUniqueId, ( ), ( const, override ) );
   MOCK_METHOD( void, SetUniqueId, ( int ), ( override ) );
   MOCK_METHOD( MegaManLofi::EntityType, GetEntityType, ( ), ( const, override ) );
   MOCK_METHOD( int, GetEntityMetaId, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Coordinate<long long>&, GetArenaPosition, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetArenaPositionLeft, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetArenaPositionTop, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityX, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityY, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::Direction, GetDirection, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Rectangle<long long>&, GetHitBox, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsMoving, ( ), ( const, override ) );
   MOCK_METHOD( void, SetEntityType, ( MegaManLofi::EntityType ), ( override ) );
   MOCK_METHOD( void, SetEntityMetaId, ( int ), ( override ) );
   MOCK_METHOD( void, SetArenaPosition, ( MegaManLofi::Coordinate<long long>), ( override ) );
   MOCK_METHOD( void, SetVelocityX, ( long long ), ( override ) );
   MOCK_METHOD( void, SetVelocityY, ( long long ), ( override ) );
   MOCK_METHOD( void, SetDirection, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( void, SetHitBox, ( MegaManLofi::Rectangle<long long> ), ( override ) );
   MOCK_METHOD( void, StopX, ( ), ( override ) );
   MOCK_METHOD( void, StopY, ( ), ( override ) );
};
