#include "gtest/gtest.h"

#include <MegaManLofi/PlayerPhysics.h>
#include <MegaManLofi/PlayerPhysicsConfig.h>
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
      _config.reset( new PlayerPhysicsConfig );

      _config->MaxPushVelocity = 10;
      _config->MaxGravityVelocity = 20;
      _config->PushAccelerationPerSecond = 2;
      _config->FrictionDecelerationPerSecond = 2;
      _config->JumpAccelerationPerSecond = 1;
      _config->GravityAccelerationPerSecond = 4;
      _config->MaxJumpExtensionSeconds = 0.25;

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( false ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( false ) );
      ON_CALL( *_playerMock, IsStanding() ).WillByDefault( Return( true ) );
   }

   void BuildPhysics()
   {
      _physics.reset( new PlayerPhysics( _frameRateProviderMock, _frameActionRegistryMock, _config ) );
      _physics->AssignTo( _playerMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<PlayerPhysicsConfig> _config;

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

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -4 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -2 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingRight_SlowsDownCorrectly )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 4 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 2 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsJumping_DoesNotApplyGravity )
{
   BuildPhysics();

   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingUp_DecreasesYVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -10 ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( -6 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsMovingDown_DecreasesYVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 10 ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 14 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsNearTerminalVelocity_ClampsToTerminalVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 19 ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 20 ) );

   _physics->Tick();
}

TEST_F( PlayerPhysicsTests, Tick_PlayerIsAtTerminalVelocity_DoesNotChangeYVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 20 ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

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

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -6 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -8 ) );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_LeftAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -9 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -10 ) );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_LeftAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -10 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->PushTo( Direction::Left );
}

TEST_F( PlayerPhysicsTests, PushTo_Right_ChangesXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 6 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 8 ) );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, PushTo_RightAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 9 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 10 ) );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, PushTo_RightAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 10 ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->PushTo( Direction::Right );
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsNotStanding_DoesNotChangeVelocity )
{
   BuildPhysics();

   ON_CALL( *_playerMock, IsStanding() ).WillByDefault( Return( false  ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsNotStanding_DoesNotFlagAction )
{
   BuildPhysics();

   ON_CALL( *_playerMock, IsStanding() ).WillByDefault( Return( false ) );

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( _ ) ).Times( 0 );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsStanding_SetsVelocityToUpwardJumpVelocity )
{
   BuildPhysics();

   EXPECT_CALL( *_playerMock, SetVelocityY( -1 ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsStanding_FlagsJumpingAction )
{
   BuildPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_PlayerIsStanding_SetsIsJumpingToTrue )
{
   BuildPhysics();

   EXPECT_CALL( *_playerMock, SetIsJumping( true ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, ExtendJump_PlayerIsNotJumping_DoesNotExtendJump )
{
   BuildPhysics();

   ON_CALL( *_frameRateProviderMock, GetCurrentFrame() ).WillByDefault( Return( 0 ) );
   ON_CALL( *_playerMock, IsJumping() ).WillByDefault( Return( false ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->ExtendJump();
}

TEST_F( PlayerPhysicsTests, ExtendJump_LastExtendJumpFrameIsTooOld_DoesNotExtendJump )
{
   BuildPhysics();

   EXPECT_CALL( *_playerMock, IsJumping() ).WillOnce( Return( true ) );
   EXPECT_CALL( *_frameRateProviderMock, GetCurrentFrame() ).WillOnce( Return( 2 ) );
   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->ExtendJump();
}

TEST_F( PlayerPhysicsTests, ExtendJump_AlreadyFullyExtendedJump_DoesNotExtendJump )
{
   _config->MaxJumpExtensionSeconds = 0;
   BuildPhysics();

   EXPECT_CALL( *_playerMock, IsJumping() ).WillOnce( Return( true ) );
   EXPECT_CALL( *_frameRateProviderMock, GetCurrentFrame() ).WillOnce( Return( 1 ) );
   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   EXPECT_CALL( *_playerMock, SetIsJumping( false ) );

   _physics->ExtendJump();
}

TEST_F( PlayerPhysicsTests, ExtendJump_ExtensionIsAllowed_ExtendsJump )
{
   BuildPhysics();

   EXPECT_CALL( *_playerMock, IsJumping() ).WillOnce( Return( true ) );
   EXPECT_CALL( *_frameRateProviderMock, GetCurrentFrame() ).WillRepeatedly( Return( 1 ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( -1 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _physics->ExtendJump();
}
