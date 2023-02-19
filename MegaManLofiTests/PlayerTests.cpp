#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Player.h>
#include <MegaManLofi/PlayerDefs.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>

#include "mock_FrameActionRegistry.h"
#include "mock_FrameRateProvider.h"
#include "mock_GameEventAggregator.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayerTests : public Test
{
public:
   void SetUp() override
   {
      _playerDefs.reset( new PlayerDefs );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );

      _playerDefs->DefaultUniqueId = 10;
      _playerDefs->DefaultEntityMetaId = 2;
      _playerDefs->DefaultVelocityX = 0;
      _playerDefs->DefaultVelocityY = 0;
      _playerDefs->MaxGravityVelocity = 10;
      _playerDefs->GravityAccelerationPerSecond = 100;
      _playerDefs->FrictionDecelerationPerSecond = 200;
      _playerDefs->DamageInvulnerabilitySeconds = 1;
      _playerDefs->KnockBackSeconds = 0.5f;
      _playerDefs->KnockBackVelocity = 500;
      _playerDefs->MaxPushVelocity = 10;
      _playerDefs->PushAccelerationPerSecond = 2;
      _playerDefs->JumpAccelerationPerSecond = 1;
      _playerDefs->MaxJumpExtensionSeconds = 0.25f;
      _playerDefs->DefaultLives = 5;
      _playerDefs->MaxHealth = 100;
      _playerDefs->DefaultDirection = Direction::Left;
      _playerDefs->DefaultHitBox = { 0, 0, 4, 4 };
      _playerDefs->DefaultMovementType = MovementType::Airborne;

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   }

   void BuildPlayer()
   {
      _player.reset( new Player( _frameRateProviderMock, _playerDefs, _frameActionRegistryMock, _eventAggregatorMock ) );
   }

protected:
   shared_ptr<PlayerDefs> _playerDefs;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;

   int _framesPerSecond;

   shared_ptr<Player> _player;
};

TEST_F( PlayerTests, Constructor_Always_SetsDefaultPropertiesFromDefs )
{
   _playerDefs->DefaultVelocityX = 100;
   _playerDefs->DefaultVelocityY = 200;
   _playerDefs->DefaultLives = 10;
   _playerDefs->MaxHealth = 40;
   _playerDefs->DefaultDirection = Direction::Right;
   _playerDefs->DefaultHitBox = { 1, 2, 3, 4 };
   BuildPlayer();

   EXPECT_EQ( _player->GetUniqueId(), 10 );
   EXPECT_EQ( _player->GetEntityMetaId(), 2 );
   EXPECT_EQ( _player->GetArenaPosition().Left, 0 );
   EXPECT_EQ( _player->GetArenaPosition().Top, 0 );
   EXPECT_EQ( _player->GetVelocityX(), 100 );
   EXPECT_EQ( _player->GetVelocityY(), 200 );
   EXPECT_EQ( _player->GetMaxGravityVelocity(), 10 );
   EXPECT_EQ( _player->GetGravityAccelerationPerSecond(), 100 );
   EXPECT_EQ( _player->GetFrictionDecelerationPerSecond(), 200 );
   EXPECT_EQ( _player->GetDamageInvulnerabilitySeconds(), 1 );
   EXPECT_EQ( _player->GetKnockBackSeconds(), 0.5f );
   EXPECT_EQ( _player->GetKnockBackVelocity(), 500 );
   EXPECT_EQ( _player->GetLivesRemaining(), 10 );
   EXPECT_EQ( _player->GetHealth(), 40 );
   EXPECT_EQ( _player->GetMaxHealth(), 40 );
   EXPECT_EQ( _player->GetDirection(), Direction::Right );
   EXPECT_EQ( _player->GetHitBox().Left, 1 );
   EXPECT_EQ( _player->GetHitBox().Top, 2 );
   EXPECT_EQ( _player->GetHitBox().Width, 3 );
   EXPECT_EQ( _player->GetHitBox().Height, 4 );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Airborne );
   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, GetEntityType_Always_ReturnsBody )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetEntityType(), EntityType::Player );
}

TEST_F( PlayerTests, Reset_Always_ResetsDefaultPropertiesFromDefs )
{
   BuildPlayer();

   _player->SetVelocityX( 100 );
   _player->SetVelocityY( 200 );
   _player->SetDirection( Direction::Right );
   _player->SetMovementType( MovementType::Walking );

   EXPECT_EQ( _player->GetVelocityX(), 100 );
   EXPECT_EQ( _player->GetVelocityY(), 200 );
   EXPECT_EQ( _player->GetDirection(), Direction::Right );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Walking );

   _player->Reset();

   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_EQ( _player->GetVelocityY(), 0 );
   EXPECT_EQ( _player->GetDirection(), Direction::Left );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Airborne );
}

TEST_F( PlayerTests, ResetPosition_Always_ResetsPositionPropertiesFromDefs )
{
   BuildPlayer();

   _player->SetHitBox( { 100, 200, 300, 400 } );
   _player->SetVelocityX( 100 );
   _player->SetVelocityY( 200 );
   _player->SetDirection( Direction::Right );
   _player->SetMovementType( MovementType::Walking );

   _player->ResetPosition();

   EXPECT_EQ( _player->GetHitBox().Left, 0 );
   EXPECT_EQ( _player->GetHitBox().Top, 0 );
   EXPECT_EQ( _player->GetHitBox().Width, 4 );
   EXPECT_EQ( _player->GetHitBox().Height, 4 );
   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_EQ( _player->GetVelocityY(), 0 );
   EXPECT_EQ( _player->GetDirection(), Direction::Left );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Airborne );
}

TEST_F( PlayerTests, ResetHealth_Always_ResetsHealthToMaximum )
{
   BuildPlayer();
   _player->SetHealth( 3 );

   _player->ResetHealth();

   EXPECT_EQ( _player->GetHealth(), 100 );
}

TEST_F( PlayerTests, GetLivesRemaining_Always_ReturnsLivesRemaining )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetLivesRemaining(), 5 );
}

TEST_F( PlayerTests, GetDirection_Always_ReturnsDirection )
{
   _playerDefs->DefaultDirection = Direction::Right;
   BuildPlayer();

   EXPECT_EQ( _player->GetDirection(), Direction::Right );
}

TEST_F( PlayerTests, GetHitBox_Always_ReturnsHitBox )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetHitBox().Left, 0 );
   EXPECT_EQ( _player->GetHitBox().Top, 0 );
   EXPECT_EQ( _player->GetHitBox().Width, 4 );
   EXPECT_EQ( _player->GetHitBox().Height, 4 );
}

TEST_F( PlayerTests, GetArenaPosition_Always_ReturnsArenaPosition )
{
   BuildPlayer();

   _player->SetArenaPosition( { 4, 5 } );

   EXPECT_EQ( _player->GetArenaPosition().Left, 4 );
   EXPECT_EQ( _player->GetArenaPosition().Top, 5 );
}

TEST_F( PlayerTests, SetArenaPosition_Always_SetsArenaPosition )
{
   BuildPlayer();
   _player->SetArenaPosition( { 4, 5 } );

   EXPECT_EQ( _player->GetArenaPosition().Left, 4 );
   EXPECT_EQ( _player->GetArenaPosition().Top, 5 );
}

TEST_F( PlayerTests, GetVelocityX_Always_ReturnsVelocityX )
{
   BuildPlayer();

   _player->SetVelocityX( 2 );

   EXPECT_EQ( _player->GetVelocityX(), 2 );
}

TEST_F( PlayerTests, GetVelocityY_Always_ReturnsVelocityY )
{
   BuildPlayer();

   _player->SetVelocityY( 5 );

   EXPECT_EQ( _player->GetVelocityY(), 5 );
}

TEST_F( PlayerTests, StopX_Always_SetsXVelocityToZero )
{
   BuildPlayer();

   _player->SetVelocityX( 2 );
   _player->StopX();

   EXPECT_EQ( _player->GetVelocityX(), 0 );
}

TEST_F( PlayerTests, StopY_Always_SetsYVelocityToZero )
{
   BuildPlayer();

   _player->SetVelocityY( -2 );
   _player->StopY();

   EXPECT_EQ( _player->GetVelocityY(), 0 );
}

TEST_F( PlayerTests, StopY_Always_SetsIsJumpingToFalse )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->Jump();
   
   EXPECT_TRUE( _player->IsJumping() );

   _player->StopY();

   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, PointTo_Always_SetsPlayerDirection )
{
   BuildPlayer();

   _player->PointTo( Direction::Down );

   EXPECT_EQ( _player->GetDirection(), Direction::Down );
}

TEST_F( PlayerTests, PushTo_PlayerIsKnockedBack_DoesNotChangeXVelocity )
{
   BuildPlayer();
   _player->SetKnockBackSeconds( 1 );
   _player->SetKnockBackVelocity( 200 );

   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_TRUE( _player->TakeCollisionPayload( { -10, 0 }, 1 ) );
   EXPECT_EQ( _player->GetVelocityX(), 200 );

   _player->PushTo( Direction::Left );

   EXPECT_EQ( _player->GetVelocityX(), 200 );
}

TEST_F( PlayerTests, PushTo_Left_FlagsAction )
{
   BuildPlayer();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerPushed ) );

   _player->PushTo( Direction::Left );
}

TEST_F( PlayerTests, PushTo_Right_FlagsAction )
{
   BuildPlayer();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerPushed ) );

   _player->PushTo( Direction::Right );
}

TEST_F( PlayerTests, PushTo_Left_ChangesXVelocity )
{
   BuildPlayer();
   _player->SetVelocityX( -6 );

   _player->PushTo( Direction::Left );

   EXPECT_EQ( _player->GetVelocityX(), -8 );
}

TEST_F( PlayerTests, PushTo_LeftAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   BuildPlayer();
   _player->SetVelocityX( -9 );

   _player->PushTo( Direction::Left );

   EXPECT_EQ( _player->GetVelocityX(), -10 );
}

TEST_F( PlayerTests, PushTo_LeftAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   BuildPlayer();
   _player->SetVelocityX( -10 );

   _player->PushTo( Direction::Left );

   EXPECT_EQ( _player->GetVelocityX(), -10 );
}

TEST_F( PlayerTests, PushTo_Right_ChangesXVelocity )
{
   BuildPlayer();
   _player->SetVelocityX( 6 );

   _player->PushTo( Direction::Right );

   EXPECT_EQ( _player->GetVelocityX(), 8 );
}

TEST_F( PlayerTests, PushTo_RightAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   BuildPlayer();
   _player->SetVelocityX( 9 );

   _player->PushTo( Direction::Right );

   EXPECT_EQ( _player->GetVelocityX(), 10 );
}

TEST_F( PlayerTests, PushTo_RightAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   BuildPlayer();
   _player->SetVelocityX( 10 );

   _player->PushTo( Direction::Right );

   EXPECT_EQ( _player->GetVelocityX(), 10 );
}

TEST_F( PlayerTests, Jump_PlayerIsKnockedBack_DoesNotChangeVelocityOrFlagAction )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->SetVelocityY( 0 );
   _player->SetKnockBackSeconds( 1 );
   _player->SetKnockBackVelocity( 200 );

   EXPECT_TRUE( _player->TakeCollisionPayload( { -10, 0 }, 1 ) );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( _ ) ).Times( 0 );

   _player->Jump();

   EXPECT_EQ( _player->GetVelocityY(), 0 );
}

TEST_F( PlayerTests, Jump_PlayerIsKnockedBack_ResetsIsJumping )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->SetVelocityY( 0 );
   _player->SetKnockBackSeconds( 1 );
   _player->SetKnockBackVelocity( 200 );

   _player->Jump();
   EXPECT_TRUE( _player->IsJumping() );

   EXPECT_TRUE( _player->TakeCollisionPayload( { -10, 0 }, 1 ) );

   _player->Jump();

   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, Jump_PlayerIsAirborne_DoesNotChangeVelocityOrFlagAction )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Airborne );
   _player->SetVelocityY( -5 );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( _ ) ).Times( 0 );

   _player->Jump();

   EXPECT_EQ( _player->GetVelocityY(), -5 );
}

TEST_F( PlayerTests, Jump_PlayerIsStanding_SetsVelocityToUpwardJumpVelocity )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->SetVelocityY( 0 );

   _player->Jump();

   EXPECT_EQ( _player->GetVelocityY(), -1 );
}

TEST_F( PlayerTests, Jump_PlayerIsStanding_FlagsJumpingAction )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _player->Jump();
}

TEST_F( PlayerTests, Jump_PlayerIsStanding_SetsIsJumpingToTrue )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );

   _player->Jump();

   EXPECT_TRUE( _player->IsJumping() );
}

TEST_F( PlayerTests, Jump_PlayerIsWalking_SetsVelocityToUpwardJumpVelocity )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Walking );

   _player->Jump();

   EXPECT_EQ( _player->GetVelocityY(), -1 );
}

TEST_F( PlayerTests, Jump_PlayerIsWalking_FlagsJumpingAction )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Walking );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _player->Jump();
}

TEST_F( PlayerTests, Jump_PlayerIsWalking_SetsIsJumpingToTrue )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Walking );

   _player->Jump();

   EXPECT_TRUE( _player->IsJumping() );
}

TEST_F( PlayerTests, ExtendJump_PlayerIsMovingDownward_SetsIsJumpingToFalse )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->Jump();
   _player->SetVelocityY( 2 );

   _player->ExtendJump();

   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, ExtendJump_PlayerIsKnockedBack_SetsIsJumpingToFalse )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->SetKnockBackSeconds( 1 );
   _player->SetKnockBackVelocity( 200 );

   _player->Jump();
   EXPECT_TRUE( _player->IsJumping() );

   EXPECT_TRUE( _player->TakeCollisionPayload( { -10, 0 }, 1 ) );

   _player->ExtendJump();

   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, ExtendJump_StillExtendingJump_FlagsPlayerJumpingAction )
{
   BuildPlayer();
   _player->SetMovementType( MovementType::Standing );
   _player->Jump();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _player->ExtendJump();
}

TEST_F( PlayerTests, TakeCollisionPayload_TookHealthPayloadAndHealthIsZero_RaisesCollisionDeathEvent )
{
   BuildPlayer();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::CollisionDeath ) );
   EXPECT_TRUE( _player->TakeCollisionPayload( { -300, 0 }, 0 ) );
}

TEST_F( PlayerTests, TakeCollisionPayload_TookHealthPayloadAndHealthIsNonZero_DoesNotRaiseCollisionDeathEvent )
{
   BuildPlayer();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::CollisionDeath ) ).Times( 0 );
   EXPECT_TRUE( _player->TakeCollisionPayload( { -99, 0 }, 0 ) );
}

TEST_F( PlayerTests, TakeCollisionPayload_PlusLives_TakesPayload )
{
   BuildPlayer();

   EXPECT_TRUE( _player->TakeCollisionPayload( { 0, 2 }, 0 ) );
   EXPECT_EQ( _player->GetLivesRemaining(), 7 );
}

TEST_F( PlayerTests, TakeCollisionPayload_MinusLivesAndHasLivesRemaining_TakesPayloadAndDoesNotRaiseDeathEvent )
{
   BuildPlayer();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::CollisionDeath ) ).Times( 0 );

   EXPECT_TRUE( _player->TakeCollisionPayload( { 0, -4 }, 0 ) );
   EXPECT_EQ( _player->GetLivesRemaining(), 1 );
}

TEST_F( PlayerTests, TakeCollisionPayload_MinusLivesAndNoLivesRemaining_TakesPayloadAndRaisesDeathEvent )
{
   BuildPlayer();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::CollisionDeath ) );

   EXPECT_TRUE( _player->TakeCollisionPayload( { 0, -5 }, 0 ) );
   EXPECT_EQ( _player->GetLivesRemaining(), 0 );
}
