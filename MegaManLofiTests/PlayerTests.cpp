#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Player.h>
#include <MegaManLofi/PlayerDefs.h>
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
      _playerDefs.reset( new PlayerDefs );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );

      _playerDefs->DefaultUniqueId = 10;
      _playerDefs->DefaultEntityMetaId = 2;
      _playerDefs->DefaultVelocityX = 0;
      _playerDefs->DefaultVelocityY = 0;
      _playerDefs->DefaultLives = 5;
      _playerDefs->DefaultDirection = Direction::Left;
      _playerDefs->DefaultHitBox = { 0, 0, 4, 4 };
      _playerDefs->DefaultMovementType = MovementType::Airborne;

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 100.0f ) );
   }

   void BuildPlayer()
   {
      _player.reset( new Player( _playerDefs, _frameActionRegistryMock, _frameRateProviderMock ) );
   }

protected:
   shared_ptr<PlayerDefs> _playerDefs;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;

   int _framesPerSecond;

   shared_ptr<Player> _player;
};

TEST_F( PlayerTests, Constructor_Always_SetsDefaultPropertiesFromDefs )
{
   _playerDefs->DefaultVelocityX = 100;
   _playerDefs->DefaultVelocityY = 200;
   _playerDefs->DefaultLives = 10;
   _playerDefs->DefaultDirection = Direction::Right;
   _playerDefs->DefaultHitBox = { 1, 2, 3, 4 };
   BuildPlayer();

   EXPECT_EQ( _player->GetUniqueId(), 10 );
   EXPECT_EQ( _player->GetEntityMetaId(), 2 );
   EXPECT_EQ( _player->GetArenaPosition().Left, 0 );
   EXPECT_EQ( _player->GetArenaPosition().Top, 0 );
   EXPECT_EQ( _player->GetVelocityX(), 100 );
   EXPECT_EQ( _player->GetVelocityY(), 200 );
   EXPECT_EQ( _player->GetLivesRemaining(), 10 );
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

   EXPECT_EQ( _player->GetEntityType(), EntityType::Body );
}

TEST_F( PlayerTests, Reset_Always_ResetsDefaultPropertiesFromDefs )
{
   BuildPlayer();

   _player->SetVelocityX( 100 );
   _player->SetVelocityY( 200 );
   _player->SetDirection( Direction::Right );
   _player->SetMovementType( MovementType::Walking );
   _player->SetIsJumping( true );

   EXPECT_EQ( _player->GetVelocityX(), 100 );
   EXPECT_EQ( _player->GetVelocityY(), 200 );
   EXPECT_EQ( _player->GetDirection(), Direction::Right );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Walking );
   EXPECT_TRUE( _player->IsJumping() );

   _player->Reset();

   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_EQ( _player->GetVelocityY(), 0 );
   EXPECT_EQ( _player->GetDirection(), Direction::Left );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Airborne );
   EXPECT_FALSE( _player->IsJumping() );
}

TEST_F( PlayerTests, ResetPosition_Always_ResetsPositionPropertiesFromDefs )
{
   BuildPlayer();

   _player->SetHitBox( { 100, 200, 300, 400 } );
   _player->SetVelocityX( 100 );
   _player->SetVelocityY( 200 );
   _player->SetDirection( Direction::Right );
   _player->SetMovementType( MovementType::Walking );
   _player->SetIsJumping( true );

   _player->ResetPosition();

   EXPECT_EQ( _player->GetHitBox().Left, 0 );
   EXPECT_EQ( _player->GetHitBox().Top, 0 );
   EXPECT_EQ( _player->GetHitBox().Width, 4 );
   EXPECT_EQ( _player->GetHitBox().Height, 4 );
   EXPECT_EQ( _player->GetVelocityX(), 0 );
   EXPECT_EQ( _player->GetVelocityY(), 0 );
   EXPECT_EQ( _player->GetDirection(), Direction::Left );
   EXPECT_EQ( _player->GetMovementType(), MovementType::Airborne );
   EXPECT_FALSE( _player->IsJumping() );
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

   _player->SetIsJumping( true );
   
   EXPECT_TRUE( _player->IsJumping() );

   _player->StopY();

   EXPECT_FALSE( _player->IsJumping() );
}
