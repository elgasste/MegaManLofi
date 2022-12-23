#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ArenaPhysics.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>
#include <MegaManLofi/GameEvent.h>

#include "mock_FrameRateProvider.h"
#include "mock_FrameActionRegistry.h"
#include "mock_GameEventAggregator.h"
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
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _playerMock.reset( new NiceMock<mock_Player> );
      _defaultTile = { true, true, true, true, false };

      _playerArenaPosition = { 10, 8 };
      _playerHitBox = { 0, 0, 4, 6 };

      ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 0 ) );
      ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 0 ) );

      ON_CALL( *_arenaMock, GetPlayer() ).WillByDefault( Return( _playerMock ) );
      ON_CALL( *_arenaMock, GetWidth() ).WillByDefault( Return( 20 ) );
      ON_CALL( *_arenaMock, GetHeight() ).WillByDefault( Return( 16 ) );
      ON_CALL( *_arenaMock, GetTileWidth() ).WillByDefault( Return( 2 ) );
      ON_CALL( *_arenaMock, GetTileHeight() ).WillByDefault( Return( 2 ) );
      ON_CALL( *_arenaMock, GetHorizontalTiles() ).WillByDefault( Return( 10 ) );
      ON_CALL( *_arenaMock, GetVerticalTiles() ).WillByDefault( Return( 8 ) );
      ON_CALL( *_arenaMock, GetTile( _ ) ).WillByDefault( ReturnRef( _defaultTile ) );

      ON_CALL( *_frameRateProviderMock, GetSecondsPerFrame() ).WillByDefault( Return( 1 ) );
   }

   void BuildArenaPhysics()
   {
      ON_CALL( *_playerMock, GetArenaPosition() ).WillByDefault( ReturnRef( _playerArenaPosition ) );
      ON_CALL( *_playerMock, GetArenaPositionLeft() ).WillByDefault( Return( _playerArenaPosition.Left ) );
      ON_CALL( *_playerMock, GetArenaPositionTop() ).WillByDefault( Return( _playerArenaPosition.Top ) );
      ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( _playerHitBox ) );

      _arenaPhysics.reset( new ArenaPhysics( _frameRateProviderMock, _frameActionRegistryMock, _eventAggregatorMock ) );
      _arenaPhysics->AssignTo( _arenaMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_Player> _playerMock;

   ArenaTile _defaultTile;
   Rectangle<long long> _playerHitBox;
   Coordinate<long long> _playerArenaPosition;

   shared_ptr<ArenaPhysics> _arenaPhysics;
};

TEST_F( ArenaPhysicsTests, Tick_PlayerDidNotMove_DoesNotFlagMoveActions )
{
   BuildArenaPhysics();

   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) ).Times( 0 );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) ).Times( 0 );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithNoLeftTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _playerArenaPosition = { 12, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingPassableUpperLeftTile_MovesPlayer )
{
   _playerArenaPosition = { 11, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 9 ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableUpperLeftTile_StopsPlayerX )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 34 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableMiddleLeftTile_StopsPlayerX )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 44 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableLowerLeftTile_StopsPlayerX )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 54 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftCloseToArenaBoundary_StopsPlayerX )
{
   _playerArenaPosition = { 1, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 0 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftAtArenaBoundary_StopsPlayerX )
{
   _playerArenaPosition = { 0, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_LeftAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   ArenaTile tile = { false, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 44 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithNoRightTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _playerArenaPosition = { 8, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableUpperRightTile_StopsPlayerX )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 38 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 12 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableMiddleRightTile_StopsPlayerX )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 48 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 12 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableLowerRightTile_StopsPlayerX )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 58 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 12 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightCloseToArenaBoundary_StopsPlayerX )
{
   _playerArenaPosition = { 15, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 16 ) );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightAtArenaBoundary_StopsPlayerX )
{
   _playerArenaPosition = { 16, 8 };
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_RightAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
{
   ArenaTile tile = { true, true, false, true };
   ON_CALL( *_arenaMock, GetTile( 47 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopX() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithNoUpTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _playerArenaPosition = { 12, 10 };
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperLeftTile_StopsPlayerY )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 25 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 6 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperMiddleTile_StopsPlayerY )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 26 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 6 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperRightTile_StopsPlayerY )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 27 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 6 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpCloseToArenaBoundary_StopsPlayerY )
{
   _playerArenaPosition = { 12, 1 };
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 0 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpAtArenaBoundary_StopsPlayerY )
{
   _playerArenaPosition = { 12, 0 };
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_UpAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   ArenaTile tile = { true, false, true, true };
   ON_CALL( *_arenaMock, GetTile( 35 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithNoDownTileCollisions_MovesPlayerAndFlagsMoveAction )
{
   _playerArenaPosition = { 12, 6 };
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerLeftTile_StopsPlayerY )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerMiddleTile_StopsPlayerY )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 76 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerRightTile_StopsPlayerY )
{
   _playerArenaPosition = { 11, 7 };
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 77 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownCloseToArenaBoundary_RaisesPitfallEvent )
{
   _playerArenaPosition = { 12, 9 };
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 10 ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Pitfall ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownAtArenaBoundary_RaisesPitfallEvent )
{
   _playerArenaPosition = { 12, 10 };
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Pitfall ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_DownAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
{
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
   EXPECT_CALL( *_playerMock, StopY() );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_HitBottomArenaBoundary_SetsIsStandingToTrue )
{
   _playerArenaPosition = { 12, 10 };
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_NotAtAnyBottomTileBoundaries_SetsIsStandingToFalse )
{
   _playerArenaPosition = { 12, 7 };
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
   _playerArenaPosition = { 11, 8 };
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_AtLowerMiddleBottomTileBoundary_SetsIsStandingToTrue )
{
   _playerArenaPosition = { 11, 8 };
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 76 ) ).WillByDefault( ReturnRef( tile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_AtLowerRightBottomTileBoundary_SetsIsStandingToTrue )
{
   _playerArenaPosition = { 11, 8 };
   ArenaTile tile = { true, true, true, false };
   ON_CALL( *_arenaMock, GetTile( 77 ) ).WillByDefault( ReturnRef( tile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );

   _arenaPhysics->Tick();
}

TEST_F( ArenaPhysicsTests, Tick_InsideDeathTile_RaisesTileDeathEvent )
{
   ArenaTile deathTile = { true, true, true, true, true };
   ON_CALL( *_arenaMock, GetTile( 45 ) ).WillByDefault( ReturnRef( deathTile ) );
   BuildArenaPhysics();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::TileDeath ) );

   _arenaPhysics->Tick();
}
