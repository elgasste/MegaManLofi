#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IReadOnlyEntity.h>

class mock_ReadOnlyEntity : public MegaManLofi::IReadOnlyEntity
{
public:
   MOCK_METHOD( int, GetUniqueId, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::EntityType, GetEntityType, ( ), ( const, override ) );
   MOCK_METHOD( int, GetEntityMetaId, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Coordinate<long long>&, GetArenaPosition, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetArenaPositionLeft, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetArenaPositionTop, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityX, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityY, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::Direction, GetDirection, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Rectangle<long long>&, GetHitBox, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::MovementType, GetMovementType, ( ), ( const, override ) );
};
