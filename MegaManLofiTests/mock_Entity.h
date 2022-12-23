#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntity.h>

class mock_Entity : public MegaManLofi::IEntity
{
public:
   MOCK_METHOD( MegaManLofi::Coordinate<long long>, GetArenaPosition, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityX, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityY, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::Direction, GetDirection, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Rectangle<long long>&, GetHitBox, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsMoving, ( ), ( const, override ) );
};
