#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ArenaPhysics.h>
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
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _playerMock.reset( new NiceMock<mock_Player> );
      _defaultTile = { true, true, true, true };

      ON_CALL( *_arenaMock, GetWidth() ).WillByDefault( Return( 20 ) );
      ON_CALL( *_arenaMock, GetHeight() ).WillByDefault( Return( 16 ) );
      ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 10 ) );
      ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 8 ) );
      ON_CALL( *_arenaMock, GetTileWidth() ).WillByDefault( Return( 2 ) );
      ON_CALL( *_arenaMock, GetTileHeight() ).WillByDefault( Return( 2 ) );
      ON_CALL( *_arenaMock, GetHorizontalTiles() ).WillByDefault( Return( 10 ) );
      ON_CALL( *_arenaMock, GetVerticalTiles() ).WillByDefault( Return( 8 ) );
      ON_CALL( *_arenaMock, GetTile( _ ) ).WillByDefault( ReturnRef( _defaultTile ) );

      _playerHitBox = { 0, 0, 4, 6 };
      ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( _playerHitBox ) );
      ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 0 ) );
      ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 0 ) );

      ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );
   }

   void BuildArenaPhysics()
   {
      _arenaPhysics.reset( new ArenaPhysics( _frameRateProviderMock, _frameActionRegistryMock ) );
      _arenaPhysics->AssignTo( _arenaMock, _playerMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_Player> _playerMock;

   ArenaTile _defaultTile;
   Rectangle _playerHitBox;

   shared_ptr<ArenaPhysics> _arenaPhysics;
};

TEST_F( ArenaPhysicsTests, AssignTo_Always_UpdatesPlayerOccupyingTileIndices )
{
   // MUFFINS
}

TEST_F( ArenaPhysicsTests, Tick_PlayerDidNotMove_DoesNotFlagMoveActions )
{
   BuildArenaPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) ).Times( 0 );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) ).Times( 0 );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithNoLeftTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 12 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingPassableUpperLeftTile_MovesPlayer )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 9 ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableUpperLeftTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 34 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableMiddleLeftTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 44 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableLowerLeftTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 54 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftCloseToArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 1 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 0 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftAtArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 0 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 44 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithNoRightTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 8 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 10 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableUpperRightTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 38 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 12 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableMiddleRightTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 48 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 12 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableLowerRightTile_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 58 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 12 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightCloseToArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 15 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( 16 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightAtArenaBoundary_StopsPlayerX )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 16 ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 47 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionX( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithNoUpTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 10 ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperLeftTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 25 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 6 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperMiddleTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 26 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 6 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperRightTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 27 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 6 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpCloseToArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 1 ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 0 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpAtArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 0 ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 35 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithNoDownTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 6 ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerLeftTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerMiddleTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 76 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerRightTile_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 77 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 8 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownCloseToArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 9 ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( 10 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownAtArenaBoundary_StopsPlayerY )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 10 ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_arenaMock, SetPlayerPositionY( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_HitBottomArenaBoundary_SetsIsStandingToTrue )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 10 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_NotAtAnyBottomTileBoundaries_SetsIsStandingToFalse )
{
   ON_CALL( *_arenaMock, GetPlayerPositionY() ).WillByDefault( Return( 7 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) ).Times( 0 );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_AtPassableBottomTileBoundary_SetsIsStandingToFalse )
{
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) ).Times( 0 );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_AtLowerLeftBottomTileBoundary_SetsIsStandingToTrue )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_AtLowerMiddleBottomTileBoundary_SetsIsStandingToTrue )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 76 ) ).WillByDefault( ReturnRef( tile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_AtLowerRightBottomTileBoundary_SetsIsStandingToTrue )
{
   ON_CALL( *_arenaMock, GetPlayerPositionX() ).WillByDefault( Return( 11 ) );
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 77 ) ).WillByDefault( ReturnRef( tile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}
