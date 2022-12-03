#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Player.h>
#include <MegaManLofi/PlayerConfig.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>

#include "mock_FrameActionRegistry.h"
#include "mock_FrameRateProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayerTests : public Test
{
public:
   void SetUp() override
   {
      _config.reset( new PlayerConfig );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );

      _config->HitBox = { 0., 0., 4., 4. };
      _config->StartVelocityX = 0.;
      _config->StartVelocityY = 0.;
      _config->MaxPushVelocity = 100.;
      _config->MaxGravityVelocity = 100.;
      _config->PushAccelerationPerSecond = 200.;
      _config->GravityAccelerationPerSecond = 200.;
      _config->StartDirection = Direction::Left;

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 100 ) );
   }

   void BuildPlayer()
   {
      _player.reset( new Player( _config, _frameActionRegistryMock, _frameRateProviderMock ) );
   }

protected:
   shared_ptr<PlayerConfig> _config;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;

   int _framesPerSecond;

   shared_ptr<Player> _player;
};

TEST_F( PlayerTests, Constructor_Always_SetsDefaultPropertiesFromConfig )
{
   _config->StartVelocityX = 100.;
   _config->StartVelocityY = 200.;
   _config->StartDirection = Direction::Right;
   BuildPlayer();

   EXPECT_EQ( _player->GetVelocityX(), 100. );
   EXPECT_EQ( _player->GetVelocityY(), 200. );
   EXPECT_EQ( _player->GetDirection(), Direction::Right );
}

TEST_F( PlayerTests, GetDirection_Always_ReturnsDirection )
{
   _config->StartDirection = Direction::Right;
   BuildPlayer();

   EXPECT_EQ( _player->GetDirection(), Direction::Right );
}

TEST_F( PlayerTests, IsMoving_NotMoving_ReturnsFalse )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetVelocityX(), 0. );
   EXPECT_EQ( _player->GetVelocityY(), 0. );
   EXPECT_FALSE( _player->IsMoving() );
}

TEST_F( PlayerTests, IsMoving_MovingHorizontally_ReturnsTrue )
{
   BuildPlayer();

   _player->SetVelocityX( -2. );

   EXPECT_TRUE( _player->IsMoving() );
}

TEST_F( PlayerTests, IsMoving_MovingVertically_ReturnsTrue )
{
   BuildPlayer();

   _player->SetVelocityY( 2. );

   EXPECT_TRUE( _player->IsMoving() );
}

TEST_F( PlayerTests, GetHitBox_Always_ReturnsHitBox )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetHitBox().Left, 0. );
   EXPECT_EQ( _player->GetHitBox().Top, 0. );
   EXPECT_EQ( _player->GetHitBox().Width, 4. );
   EXPECT_EQ( _player->GetHitBox().Height, 4. );
}

TEST_F( PlayerTests, GetVelocityX_Always_ReturnsVelocityX )
{
   BuildPlayer();

   _player->SetVelocityX( 2. );

   EXPECT_EQ( _player->GetVelocityX(), 2. );
}

TEST_F( PlayerTests, GetVelocityY_Always_ReturnsVelocityY )
{
   BuildPlayer();

   _player->SetVelocityY( 5. );

   EXPECT_EQ( _player->GetVelocityY(), 5. );
}

TEST_F( PlayerTests, Point_Always_SetsDirectionToSpecifiedValue )
{
   BuildPlayer();

   _player->Point( Direction::DownRight );

   EXPECT_EQ( _player->GetDirection(), Direction::DownRight );
}

TEST_F( PlayerTests, StopX_Always_SetsXVelocityToZero )
{
   BuildPlayer();

   _player->SetVelocityX( 2. );
   _player->StopX();

   EXPECT_EQ( _player->GetVelocityX(), 0. );
}

TEST_F( PlayerTests, StopY_Always_SetsYVelocityToZero )
{
   BuildPlayer();

   _player->SetVelocityY( -2. );
   _player->StopY();

   EXPECT_EQ( _player->GetVelocityY(), 0. );
}
