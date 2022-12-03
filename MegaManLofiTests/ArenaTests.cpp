#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Arena.h>
#include <MegaManLofi/ArenaConfig.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>

#include "mock_Player.h"
#include "mock_FrameActionRegistry.h"
#include "mock_FrameRateProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ArenaTests : public Test
{
public:
   void SetUp() override
   {
      _config.reset( new ArenaConfig );
      _playerMock.reset( new NiceMock<mock_Player> );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );

      _config->TileWidth = 2.;
      _config->TileHeight = 2.;
      _config->HorizontalTiles = 10;
      _config->VerticalTiles = 8;
      _config->PlayerStartX = 10.;
      _config->PlayerStartY = 8.;

      _playerHitBox = { 0., 0., 4., 6. };
      ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( _playerHitBox ) );
      ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 0. ) );
      ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 0. ) );

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );

      for ( int i = 0; i < _config->HorizontalTiles * _config->VerticalTiles; i++ )
      {
         _config->Tiles.push_back( { true, true, true, true } );
      }
   }

   void BuildArena()
   {
      _arena.reset( new Arena( _config, _playerMock, _frameActionRegistryMock, _frameRateProviderMock ) );
   }

protected:
   shared_ptr<ArenaConfig> _config;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;

   shared_ptr<Arena> _arena;
   Rectangle _playerHitBox;
};

TEST_F( ArenaTests, Constructor_Always_SetsDefaultInfoBasedOnConfig )
{
   BuildArena();

   EXPECT_EQ( _arena->GetWidth(), 20. );
   EXPECT_EQ( _arena->GetHeight(), 16. );
   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_PlayerDidNotMove_DoesNotFlagMoveActions )
{
   BuildArena();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) ).Times( 0 );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) ).Times( 0 );

   _arena->MovePlayer();
}

TEST_F( ArenaTests, MovePlayer_LeftWithNoLeftTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _config->PlayerStartX = 12.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_UpWithNoUpTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _config->PlayerStartY = 10.;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_RightWithNoRightTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _config->PlayerStartX = 8.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_DownWithNoDownTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _config->PlayerStartY = 6.;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_LeftWithCollidingPassableUpperLeftTile_MovesPlayer )
{
   _config->PlayerStartX = 11.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 9. );
}

TEST_F( ArenaTests, MovePlayer_LeftWithCollidingNonPassableUpperLeftTile_StopsPlayerX )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[34].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_LeftWithCollidingNonPassableMiddleLeftTile_StopsPlayerX )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[44].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_LeftWithCollidingNonPassableLowerLeftTile_StopsPlayerX )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[54].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_LeftCloseToArenaBoundary_StopsPlayerX )
{
   _config->PlayerStartX = 1.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 0. );
}

TEST_F( ArenaTests, MovePlayer_LeftAtArenaBoundary_StopsPlayerX )
{
   _config->PlayerStartX = 0.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 0. );
}

TEST_F( ArenaTests, MovePlayer_LeftAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   _config->Tiles[44].LeftPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_RightWithCollidingNonPassableUpperRightTile_StopsPlayerX )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[38].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 12. );
}

TEST_F( ArenaTests, MovePlayer_RightWithCollidingNonPassableMiddleRightTile_StopsPlayerX )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[48].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 12. );
}

TEST_F( ArenaTests, MovePlayer_RightWithCollidingNonPassableLowerRightTile_StopsPlayerX )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[58].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 12. );
}

TEST_F( ArenaTests, MovePlayer_RightCloseToArenaBoundary_StopsPlayerX )
{
   _config->PlayerStartX = 15.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 16. );
}

TEST_F( ArenaTests, MovePlayer_RightAtArenaBoundary_StopsPlayerX )
{
   _config->PlayerStartX = 16.;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 16. );
}

TEST_F( ArenaTests, MovePlayer_RightAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   _config->Tiles[47].RightPassable = false;
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopX() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
}

TEST_F( ArenaTests, MovePlayer_UpWithCollidingNonPassableUpperLeftTile_StopsPlayerY )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[25].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 6. );
}

TEST_F( ArenaTests, MovePlayer_UpWithCollidingNonPassableUpperMiddleTile_StopsPlayerY )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[26].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 6. );
}

TEST_F( ArenaTests, MovePlayer_UpWithCollidingNonPassableUpperRightTile_StopsPlayerY )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[27].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 6. );
}

TEST_F( ArenaTests, MovePlayer_UpCloseToArenaBoundary_StopsPlayerY )
{
   _config->PlayerStartY = 1.;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 0. );
}

TEST_F( ArenaTests, MovePlayer_UpAtArenaBoundary_StopsPlayerY )
{
   _config->PlayerStartY = 0.;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 0. );
}

TEST_F( ArenaTests, MovePlayer_UpAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   _config->Tiles[35].UpPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_DownWithCollidingNonPassableLowerLeftTile_StopsPlayerY )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[75].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_DownWithCollidingNonPassableLowerMiddleTile_StopsPlayerY )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[76].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_DownWithCollidingNonPassableLowerRightTile_StopsPlayerY )
{
   _config->PlayerStartX = 11.;
   _config->PlayerStartY = 7.;
   _config->Tiles[77].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}

TEST_F( ArenaTests, MovePlayer_DownCloseToArenaBoundary_StopsPlayerY )
{
   _config->PlayerStartY = 9.;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 10. );
}

TEST_F( ArenaTests, MovePlayer_DownAtArenaBoundary_StopsPlayerY )
{
   _config->PlayerStartY = 10.;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 10. );
}

TEST_F( ArenaTests, MovePlayer_DownAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   _config->Tiles[75].DownPassable = false;
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2. ) );
   BuildArena();

   EXPECT_CALL( *_playerMock, StopY() );

   _arena->MovePlayer();

   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
}
