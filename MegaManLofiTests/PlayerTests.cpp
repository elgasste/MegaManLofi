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

      _config->DefaultVelocityX = 0;
      _config->DefaultVelocityY = 0;
      _config->DefaultLives = 5;
      _config->DefaultDirection = Direction::Left;
      _config->DefaultHitBox = { 0, 0, 4, 4 };

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
   _config->DefaultVelocityX = 100;
   _config->DefaultVelocityY = 200;
   _config->DefaultLives = 10;
   _config->DefaultDirection = Direction::Right;
   _config->DefaultHitBox = { 1, 2, 3, 4 };
   BuildPlayer();

   EXPECT_EQ( _player->GetVelocityX(), 100 );
   EXPECT_EQ( _player->GetVelocityY(), 200 );
   EXPECT_EQ( _player->GetLivesRemaining(), 10 );
   EXPECT_EQ( _player->GetDirection(), Direction::Right );
   EXPECT_EQ( _player->GetHitBox().Left, 1 );
   EXPECT_EQ( _player->GetHitBox().Top, 2 );
   EXPECT_EQ( _player->GetHitBox().Width, 3 );
   EXPECT_EQ( _player->GetHitBox().Height, 4 );
   EXPECT_FALSE( _player->IsStanding() );
   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, Reset_Always_ResetsDefaultPropertiesFromConfig )
{
   BuildPlayer();

   _player->SetVelocityX( 100 );
   _player->SetVelocityY( 200 );
   _player->SetDirection( Direction::Right );
   _player->SetIsStanding( true );
   _player->SetIsJumping( true );

   EXPECT_EQ( _player->GetVelocityX(), 100 );
   EXPECT_EQ( _player->GetVelocityY(), 200 );
   EXPECT_EQ( _player->GetDirection(), Direction::Right );
   EXPECT_TRUE( _player->IsStanding() );
   EXPECT_TRUE( _player->IsJumping() );

   _player->Reset();

   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_EQ( _player->GetVelocityY(), 0 );
   EXPECT_EQ( _player->GetDirection(), Direction::Left );
   EXPECT_FALSE( _player->IsStanding() );
   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, GetLivesRemaining_Always_ReturnsLivesRemaining )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetLivesRemaining(), 5 );
}

TEST_F( PlayerTests, GetDirection_Always_ReturnsDirection )
{
   _config->DefaultDirection = Direction::Right;
   BuildPlayer();

   EXPECT_EQ( _player->GetDirection(), Direction::Right );
}

TEST_F( PlayerTests, IsMoving_NotMoving_ReturnsFalse )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_EQ( _player->GetVelocityY(), 0 );
   EXPECT_FALSE( _player->IsMoving() );
}

TEST_F( PlayerTests, IsMoving_MovingHorizontally_ReturnsTrue )
{
   BuildPlayer();

   _player->SetVelocityX( -2 );

   EXPECT_TRUE( _player->IsMoving() );
}

TEST_F( PlayerTests, IsMoving_MovingVertically_ReturnsTrue )
{
   BuildPlayer();

   _player->SetVelocityY( 2 );

   EXPECT_TRUE( _player->IsMoving() );
}

TEST_F( PlayerTests, GetHitBox_Always_ReturnsHitBox )
{
   BuildPlayer();

   EXPECT_EQ( _player->GetHitBox().Left, 0 );
   EXPECT_EQ( _player->GetHitBox().Top, 0 );
   EXPECT_EQ( _player->GetHitBox().Width, 4 );
   EXPECT_EQ( _player->GetHitBox().Height, 4 );
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

   _player->SetIsJumping( true );
   
   EXPECT_TRUE( _player->IsJumping() );

   _player->StopY();

   EXPECT_FALSE( _player->IsJumping() );
}
