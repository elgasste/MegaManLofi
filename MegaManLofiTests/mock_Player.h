#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IPlayer.h>

class mock_Player : public MegaManLofi::IPlayer
{
public:
   MOCK_METHOD( MegaManLofi::Direction, GetDirection, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsMoving, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Rectangle&, GetHitBox, ( ), ( const, override ) );
   MOCK_METHOD( double, GetVelocityX, ( ), ( const, override ) );
   MOCK_METHOD( double, GetVelocityY, ( ), ( const, override ) );
   MOCK_METHOD( void, SetVelocityX, ( double ), ( override ) );
   MOCK_METHOD( void, SetVelocityY, ( double ), ( override ) );
   MOCK_METHOD( void, Point, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( void, StopX, ( ), ( override ) );
   MOCK_METHOD( void, StopY, ( ), ( override ) );
};
