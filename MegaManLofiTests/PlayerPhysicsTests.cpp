#include "gtest/gtest.h"

#include <MegaManLofi/PlayerPhysics.h>
#include <MegaManLofi/PlayerPhysicsDefs.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Direction.h>

#include "mock_FrameRateProvider.h"
#include "mock_FrameActionRegistry.h"
#include "mock_Player.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayerPhysicsTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _playerMock.reset( new NiceMock<mock_Player> );
      _physicsDefs.reset( new PlayerPhysicsDefs );

      _physicsDefs->MaxPushVelocity = 10;
      _physicsDefs->MaxGravityVelocity = 20;
      _physicsDefs->PushAccelerationPerSecond = 2;
      _physicsDefs->FrictionDecelerationPerSecond = 2;
      _physicsDefs->JumpAccelerationPerSecond = 1;
      _physicsDefs->GravityAccelerationPerSecond = 4;
      _physicsDefs->MaxJumpExtensionSeconds = 0.25;

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( false ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( false ) );
      ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Standing ) );
   }

   void BuildPhysics()
   {
      _physics.reset( new PlayerPhysics( _frameRateProviderMock, _frameActionRegistryMock, _physicsDefs ) );
      _physics->AssignTo( _playerMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<PlayerPhysicsDefs> _physicsDefs;

   shared_ptr<PlayerPhysics> _physics;
};

TEST_F( PlayerPhysicsTests, Tick_PlayerWasPushed_DoesNotApplyFriction )
{
   BuildPhysics();

   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingLeft_SlowsDownCorrectly )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -4.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -2 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingRight_SlowsDownCorrectly )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 4.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 2 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingUpwardAndJumping_AppliesNormalGravity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2.0f ) );
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 2 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingUpwardAndNotJumping_SetsYVelocityToZero )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2.0f ) );
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( false ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 0 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingDown_IncreasesYVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 10.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 14 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsNearTerminalVelocity_ClampsToTerminalVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 19.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 20 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsAtTerminalVelocity_DoesNotChangeYVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 20.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 20 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, PointTo_Always_SetsPlayerDirection )
{
   BuildPhysics();

   EXPECT_CALL( *_playerMock, SetDirection( Direction::Down ) );

   _physics->PointTo( Direction::Down );
}

TEST_F( PlayerPhysicsTests, PushTo_Left_FlagsAction )
{
   BuildPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerPushed ) );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_Right_FlagsAction )
{
   BuildPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerPushed ) );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, PushTo_Left_ChangesXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -6.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -8 ) );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_LeftAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -9.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -10 ) );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_LeftAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -10.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_Right_ChangesXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 6.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 8 ) );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, PushTo_RightAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 9.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 10 ) );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, PushTo_RightAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 10.0f ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsAirborne_DoesNotChangeVelocityOrFlagAction )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Airborne ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( _ ) ).Times( 0 );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsStanding_SetsVelocityToUpwardJumpVelocity )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Standing ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( -1 ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsStanding_FlagsJumpingAction )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Standing ) );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsStanding_SetsIsJumpingToTrue )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Standing ) );

   EXPECT_CALL( *_playerMock, SetIsJumping( true ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsWalking_SetsVelocityToUpwardJumpVelocity )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Walking ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( -1 ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsWalking_FlagsJumpingAction )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Walking ) );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsWalking_SetsIsJumpingToTrue )
{
   BuildPhysics();
   ON_CALL( *_playerMock, GetMovementType() ).WillByDefault( Return( MovementType::Walking ) );

   EXPECT_CALL( *_playerMock, SetIsJumping( true ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, ExtendJump_PlayerIsMovingDownward_SetsIsJumpingToFalse )
{
   ON_CALL( *_playerMock, IsJumping() ).WillByDefault( Return( true ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2.0f ) );
   BuildPhysics();

   EXPECT_CALL( *_playerMock, SetIsJumping( false ) );

   _physics->ExtendJump();
}

TEST_F( PlayerPhysicsTests, ExtendJump_StillExtendingJump_FlagsPlayerJumpingAction )
{
   ON_CALL( *_playerMock, IsJumping() ).WillByDefault( Return( true ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2.0f ) );
   BuildPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _physics->ExtendJump();
}
