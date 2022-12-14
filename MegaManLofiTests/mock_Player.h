#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IPlayer.h>

class mock_Player : public MegaManLofi::IPlayer
{
public:
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( void, ResetPhysics, ( ), ( override ) );
   MOCK_METHOD( unsigned int, GetLivesRemaining, ( ), ( const, override ) );
   MOCK_METHOD( MegaManLofi::Direction, GetDirection, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Rectangle<long long>&, GetHitBox, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsMoving, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsStanding, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsJumping, ( ), ( const, override ) );
   MOCK_METHOD( void, SetLivesRemaining, ( unsigned int ), ( override ) );
   MOCK_METHOD( void, SetDirection, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( long long, GetVelocityX, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetVelocityY, ( ), ( const, override ) );
   MOCK_METHOD( void, SetVelocityX, ( long long ), ( override ) );
   MOCK_METHOD( void, SetVelocityY, ( long long ), ( override ) );
   MOCK_METHOD( void, SetIsStanding, ( bool ), ( override ) );
   MOCK_METHOD( void, SetIsJumping, ( bool ), ( override ) );
   MOCK_METHOD( void, StopX, ( ), ( override ) );
   MOCK_METHOD( void, StopY, ( ), ( override ) );
};
