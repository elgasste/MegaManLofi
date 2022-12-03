#include "gtest/gtest.h"

#include <MegaManLofi/Physics.h>
#include <MegaManLofi/PlayerConfig.h>
#include <MegaManLofi/FrameAction.h>

#include "mock_FrameRateProvider.h"
#include "mock_FrameActionRegistry.h"
#include "mock_Player.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PhysicsTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _playerConfig.reset( new PlayerConfig );
      _playerMock.reset( new NiceMock<mock_Player> );

      _playerConfig->MaxPushVelocity = 10.;
      _playerConfig->PushAccelerationPerSecond = 2.;

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( false ) );

      _physics.reset( new Physics( _frameRateProviderMock, _frameActionRegistryMock, _playerConfig ) );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<PlayerConfig> _playerConfig;
   shared_ptr<mock_Player> _playerMock;

   shared_ptr<Physics> _physics;
};

TEST_F( PhysicsTests, PlayerApplyFriction_PlayerWasPushed_DoesNotApplyFriction )
{
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );
}

TEST_F( PhysicsTests, PlayerApplyFriction_PlayerIsMovingLeft_SlowsDownCorrectly )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -4. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -2. ) );

   _physics->PlayerApplyFriction( _playerMock );
}

TEST_F( PhysicsTests, PlayerApplyFriction_PlayerIsMovingRight_SlowsDownCorrectly )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 4. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 2. ) );

   _physics->PlayerApplyFriction( _playerMock );
}

TEST_F( PhysicsTests, PlayerApplyFriction_PlayerIsMovingLeftTooFast_ClampsToMaximumVelocity )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -14. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( -10. ) );

   _physics->PlayerApplyFriction( _playerMock );
}

TEST_F( PhysicsTests, PlayerApplyFriction_PlayerIsMovingRightAndHasNoVelocityToSpare_Stops )
{
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 14. ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( 10. ) );

   _physics->PlayerApplyFriction( _playerMock );
}
