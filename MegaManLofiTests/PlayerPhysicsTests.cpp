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

      _config->MaxPushVelocity = 10.;
      _config->MaxGravityVelocity = 20.;
      _config->PushAccelerationPerSecond = 2.;
      _config->FrictionDecelerationPerSecond = 2.;
      _config->GravityAccelerationPerSecond = 4.;

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( false ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( false ) );

      _physics.reset( new PlayerPhysics( _frameRateProviderMock, _frameActionRegistryMock, _playerMock, _config ) );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<PlayerPhysicsConfig> _config;

   shared_ptr<PlayerPhysics> _physics;
};

TEST_F( PlayerPhysicsTests, ApplyFriction_PlayerWasPushed_DoesNotApplyFriction )
{
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );
}

TEST_F( PlayerPhysicsTests, ApplyFriction_PlayerIsMovingLeft_SlowsDownCorrectly )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -4. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -2. ) );

   _physics->ApplyFriction();
}

TEST_F( PlayerPhysicsTests, ApplyFriction_PlayerIsMovingRight_SlowsDownCorrectly )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 4. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 2. ) );

   _physics->ApplyFriction();
}

TEST_F( PlayerPhysicsTests, ApplyGravity_PlayerIsJumping_DoesNotApplyGravity )
{
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->ApplyGravity();
}

TEST_F( PlayerPhysicsTests, ApplyGravity_PlayerIsMovingUp_DecreasesYVelocity )
{
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -10. ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( -6. ) );

   _physics->ApplyGravity();
}

TEST_F( PlayerPhysicsTests, ApplyGravity_PlayerIsMovingDown_DecreasesYVelocity )
{
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 10. ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 14. ) );

   _physics->ApplyGravity();
}

TEST_F( PlayerPhysicsTests, ApplyGravity_PlayerIsNearTerminalVelocity_ClampsToTerminalVelocity )
{
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 19. ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( 20. ) );

   _physics->ApplyGravity();
}

TEST_F( PlayerPhysicsTests, ApplyGravity_PlayerIsAtTerminalVelocity_DoesNotChangeYVelocity )
{
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 20. ) );

   EXPECT_CALL( *_playerMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->ApplyGravity();
}

TEST_F( PlayerPhysicsTests, Point_Always_SetsPlayerDirection )
{
   EXPECT_CALL( *_playerMock, SetDirection( Direction::Down ) );

   _physics->Point( Direction::Down );
}

TEST_F( PlayerPhysicsTests, Push_Left_FlagsAction )
{
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerPushed ) );

   _physics->Push( Direction::Left );
}

TEST_F( PlayerPhysicsTests, Push_Right_FlagsAction )
{
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerPushed ) );

   _physics->Push( Direction::Right );
}

TEST_F( PlayerPhysicsTests, Push_Left_ChangesXVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -6. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -8. ) );

   _physics->Push( Direction::Left );
}

TEST_F( PlayerPhysicsTests, Push_LeftAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -9. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -10. ) );

   _physics->Push( Direction::Left );
}

TEST_F( PlayerPhysicsTests, Push_LeftAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -10. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->Push( Direction::Left );
}

TEST_F( PlayerPhysicsTests, Push_Right_ChangesXVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 6. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 8. ) );

   _physics->Push( Direction::Right );
}

TEST_F( PlayerPhysicsTests, Push_RightAndPushVelocityHasAlmostMaxedOut_ClampsToMaxPushVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 9. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 10. ) );

   _physics->Push( Direction::Right );
}

TEST_F( PlayerPhysicsTests, Push_RightAndPushVelocityHasMaxedOut_DoesNotChangeXVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 10. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->Push( Direction::Right );
}

TEST_F( PlayerPhysicsTests, Jump_Always_SetsVelocityToUpwardGravityMaximum )
{
   EXPECT_CALL( *_playerMock, SetVelocityY( -20. ) );

   _physics->Jump();
}

TEST_F( PlayerPhysicsTests, Jump_Always_FlagsJumpingAction )
{
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerJumping ) );

   _physics->Jump();
}
