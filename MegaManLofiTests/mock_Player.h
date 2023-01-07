#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/Player.h>

class mock_Player : public MegaManLofi::Player
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

   MOCK_METHOD( void, SetUniqueId, ( int ), ( override ) );
   MOCK_METHOD( void, SetEntityType, ( MegaManLofi::EntityType ), ( override ) );
   MOCK_METHOD( void, SetEntityMetaId, ( int ), ( override ) );
   MOCK_METHOD( void, SetArenaPosition, ( MegaManLofi::Coordinate<float> ), ( override ) );
   MOCK_METHOD( void, SetVelocityX, ( float ), ( override ) );
   MOCK_METHOD( void, SetVelocityY, ( float ), ( override ) );
   MOCK_METHOD( void, SetDirection, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( void, SetHitBox, ( MegaManLofi::Rectangle<float> ), ( override ) );
   MOCK_METHOD( void, SetMovementType, ( MegaManLofi::MovementType ), ( override ) );
   MOCK_METHOD( void, StopX, ( ), ( override ) );
   MOCK_METHOD( void, StopY, ( ), ( override ) );

   MOCK_METHOD( unsigned int, GetLivesRemaining, ( ), ( const, override ) );
   MOCK_METHOD( bool, IsJumping, ( ), ( const, override ) );

   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( void, ResetPosition, ( ), ( override ) );
   MOCK_METHOD( void, SetLivesRemaining, ( unsigned int ), ( override ) );
   MOCK_METHOD( void, PointTo, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( void, PushTo, ( MegaManLofi::Direction ), ( override ) );
   MOCK_METHOD( void, Jump, ( ), ( override ) );
   MOCK_METHOD( void, ExtendJump, ( ), ( override ) );
};
