#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ArenaPhysics.h>
#include <MegaManLofi/ArenaConfig.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>

#include "mock_FrameRateProvider.h"
#include "mock_FrameActionRegistry.h"
#include "mock_Arena.h"
#include "mock_Player.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ArenaPhysicsTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _arenaConfig.reset( new ArenaConfig );
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _playerMock.reset( new NiceMock<mock_Player> );

      _arenaConfig->TileWidth = 2.;
      _arenaConfig->TileHeight = 2.;
      _arenaConfig->HorizontalTiles = 10;
      _arenaConfig->VerticalTiles = 8;

      ON_CALL( *_arenaMock, GetWidth() ).WillByDefault( Return( 20. ) );
      ON_CALL( *_arenaMock, GetHeight() ).WillByDefault( Return( 16. ) );
      ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 10. ) );
      ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 8. ) );

      _playerHitBox = { 0., 0., 4., 6. };
      ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( _playerHitBox ) );
      ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 0. ) );
      ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 0. ) );

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );

      for ( int i = 0; i < _arenaConfig->HorizontalTiles * _arenaConfig->VerticalTiles; i++ )
      {
         _arenaConfig->Tiles.push_back( { true, true, true, true } );
      }
   }

   void BuildArenaPhysics()
   {
      _arenaPhysics.reset( new ArenaPhysics( _frameRateProviderMock, _frameActionRegistryMock, _arenaConfig, _arenaMock, _playerMock ) );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<ArenaConfig> _arenaConfig;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_Player> _playerMock;

   Rectangle _playerHitBox;

   shared_ptr<ArenaPhysics> _arenaPhysics;
};

TEST_F( ArenaPhysicsTests, MovePlayer_PlayerDidNotMove_DoesNotFlagMoveActions )
{
   BuildArenaPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) ).Times( 0 );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) ).Times( 0 );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftWithNoLeftTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 12. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10. ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpWithNoUpTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 10. ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8. ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightWithNoRightTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 8. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10. ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownWithNoDownTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 6. ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8. ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftWithCollidingPassableUpperLeftTile_MovesPlayer )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 9. ) );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftWithCollidingNonPassableUpperLeftTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[34].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftWithCollidingNonPassableMiddleLeftTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[44].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftWithCollidingNonPassableLowerLeftTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[54].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftCloseToArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 1. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 0. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftAtArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 0. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_LeftAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   _arenaConfig->Tiles[44].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightWithCollidingNonPassableUpperRightTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[38].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 12. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightWithCollidingNonPassableMiddleRightTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[48].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 12. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightWithCollidingNonPassableLowerRightTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[58].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 12. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightCloseToArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 15. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 16. ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightAtArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 16. ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_RightAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   _arenaConfig->Tiles[47].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpWithCollidingNonPassableUpperLeftTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[25].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 6. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpWithCollidingNonPassableUpperMiddleTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[26].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 6. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpWithCollidingNonPassableUpperRightTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[27].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 6. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpCloseToArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 1. ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 0. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpAtArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 0. ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_UpAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   _arenaConfig->Tiles[35].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownWithCollidingNonPassableLowerLeftTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[75].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownWithCollidingNonPassableLowerMiddleTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[76].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownWithCollidingNonPassableLowerRightTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11. ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7. ) );
   _arenaConfig->Tiles[77].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownCloseToArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 9. ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 10. ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownAtArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 10. ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}

TEST_F( ArenaPhysicsTests, MovePlayer_DownAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   _arenaConfig->Tiles[75].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->MovePlayer();
}
