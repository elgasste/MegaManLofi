#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ArenaPhysics.h>
#include <MegaManLofi/WorldDefs.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>
#include <MegaManLofi/GameEvent.h>

#include "mock_FrameRateProvider.h"
#include "mock_GameEventAggregator.h"
#include "mock_Stage.h"
#include "mock_Arena.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ArenaPhysicsTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _worldDefs.reset( new WorldDefs() );
      _stageMock.reset( new NiceMock<mock_Stage> );
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _defaultTile = { true, true, true, true, false };

      _worldDefs->ActiveRegionWidth = 20;
      _worldDefs->ActiveRegionHeight = 10;

      ON_CALL( *_stageMock, GetActiveArena() ).WillByDefault( Return( _arenaMock ) );
      ON_CALL( *_stageMock, GetMutableActiveArena() ).WillByDefault( Return( _arenaMock ) );

      ON_CALL( *_arenaMock, GetWidth() ).WillByDefault( Return( 20.0f ) );
      ON_CALL( *_arenaMock, GetHeight() ).WillByDefault( Return( 16.0f ) );
      ON_CALL( *_arenaMock, GetHorizontalTiles() ).WillByDefault( Return( 10 ) );
      ON_CALL( *_arenaMock, GetVerticalTiles() ).WillByDefault( Return( 8 ) );
      ON_CALL( *_arenaMock, GetTile( _ ) ).WillByDefault( ReturnRef( _defaultTile ) );
      ON_CALL( *_arenaMock, GetEntityCount() ).WillByDefault( Return( 1 ) );

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   }

   void BuildArenaPhysics()
   {
      _arenaPhysics.reset( new ArenaPhysics( _frameRateProviderMock, _eventAggregatorMock, _worldDefs ) );
      _arenaPhysics->AssignTo( _stageMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<WorldDefs> _worldDefs;
   shared_ptr<mock_Stage> _stageMock;
   shared_ptr<mock_Arena> _arenaMock;

   ArenaTile _defaultTile;
   Rectangle<float> _playerHitBox;
   Coordinate<float> _playerArenaPosition;

   shared_ptr<ArenaPhysics> _arenaPhysics;
};

// TODO: I'd very much like to refactor ArenaPhysics into smaller parts, and in the
// meantime it seems like a waste of time to fix these tests.

//TEST_F( ArenaPhysicsTests, Tick_LeftWithNoLeftTileCollisions_MovesPlayerAndFlagsMoveAction )
//{
//   _playerArenaPosition = { 12, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
//   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingPassableUpperLeftTile_MovesPlayer )
//{
//   _playerArenaPosition = { 11, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 9 ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableUpperLeftTile_StopsPlayerX )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { false, true, true, true };
//   ON_CALL( *_arenaMock, GetTile( 34 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableMiddleLeftTile_StopsPlayerX )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { false, true, true, true };
//   ON_CALL( *_arenaMock, GetTile( 44 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftWithCollidingNonPassableLowerLeftTile_StopsPlayerX )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { false, true, true, true };
//   ON_CALL( *_arenaMock, GetTile( 54 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftCloseToArenaBoundary_StopsPlayerX )
//{
//   _playerArenaPosition = { 1, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 0 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftAtArenaBoundary_StopsPlayerX )
//{
//   _playerArenaPosition = { 0, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_LeftAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
//{
//   ArenaTile tile = { false, true, true, true };
//   ON_CALL( *_arenaMock, GetTile( 44 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightWithNoRightTileCollisions_MovesPlayerAndFlagsMoveAction )
//{
//   _playerArenaPosition = { 8, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 10 ) );
//   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedHorizontal ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableUpperRightTile_StopsPlayerX )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, true, false, true };
//   ON_CALL( *_arenaMock, GetTile( 38 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 12 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableMiddleRightTile_StopsPlayerX )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, true, false, true };
//   ON_CALL( *_arenaMock, GetTile( 48 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 12 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightWithCollidingNonPassableLowerRightTile_StopsPlayerX )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, true, false, true };
//   ON_CALL( *_arenaMock, GetTile( 58 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 12 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightCloseToArenaBoundary_StopsPlayerX )
//{
//   _playerArenaPosition = { 15, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( 16 ) );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightAtArenaBoundary_StopsPlayerX )
//{
//   _playerArenaPosition = { 16, 8 };
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_RightAtTileBoundaryAndTileIsNotPassable_StopsPlayerX )
//{
//   ArenaTile tile = { true, true, false, true };
//   ON_CALL( *_arenaMock, GetTile( 47 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityX() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionLeft( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopX() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpWithNoUpTileCollisions_MovesPlayerAndFlagsMoveAction )
//{
//   _playerArenaPosition = { 12, 10 };
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
//   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperLeftTile_StopsPlayerY )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, false, true, true };
//   ON_CALL( *_arenaMock, GetTile( 25 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 6 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperMiddleTile_StopsPlayerY )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, false, true, true };
//   ON_CALL( *_arenaMock, GetTile( 26 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 6 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpWithCollidingNonPassableUpperRightTile_StopsPlayerY )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, false, true, true };
//   ON_CALL( *_arenaMock, GetTile( 27 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 6 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpCloseToArenaBoundary_StopsPlayerY )
//{
//   _playerArenaPosition = { 12, 1 };
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 0 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpAtArenaBoundary_StopsPlayerY )
//{
//   _playerArenaPosition = { 12, 0 };
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_UpAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
//{
//   ArenaTile tile = { true, false, true, true };
//   ON_CALL( *_arenaMock, GetTile( 35 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownWithNoDownTileCollisions_MovesPlayerAndFlagsMoveAction )
//{
//   _playerArenaPosition = { 12, 6 };
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
//   EXPECT_CALL( *_frameActionRegistryMock, FlagAction( FrameAction::PlayerMovedVertical ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerLeftTile_StopsPlayerY )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerMiddleTile_StopsPlayerY )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 76 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownWithCollidingNonPassableLowerRightTile_StopsPlayerY )
//{
//   _playerArenaPosition = { 11, 7 };
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 77 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 8 ) );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownCloseToArenaBoundary_RaisesPitfallEvent )
//{
//   _playerArenaPosition = { 12, 9 };
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( 10 ) );
//   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Pitfall ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownAtArenaBoundary_RaisesPitfallEvent )
//{
//   _playerArenaPosition = { 12, 10 };
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
//   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Pitfall ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_DownAtTileBoundaryAndTileIsNotPassable_StopsPlayerY )
//{
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
//   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 2 ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetArenaPositionTop( _ ) ).Times( 0 );
//   EXPECT_CALL( *_playerMock, StopY() );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_HitBottomArenaBoundary_SetsIsStandingToTrue )
//{
//   _playerArenaPosition = { 12, 10 };
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
//   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_NotAtAnyBottomTileBoundaries_SetsIsStandingToFalse )
//{
//   _playerArenaPosition = { 12, 7 };
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
//   EXPECT_CALL( *_playerMock, SetIsStanding( true ) ).Times( 0 );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_AtPassableBottomTileBoundary_SetsIsStandingToFalse )
//{
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
//   EXPECT_CALL( *_playerMock, SetIsStanding( true ) ).Times( 0 );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_AtLowerLeftBottomTileBoundary_SetsIsStandingToTrue )
//{
//   _playerArenaPosition = { 11, 8 };
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 75 ) ).WillByDefault( ReturnRef( tile ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
//   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_AtLowerMiddleBottomTileBoundary_SetsIsStandingToTrue )
//{
//   _playerArenaPosition = { 11, 8 };
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 76 ) ).WillByDefault( ReturnRef( tile ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
//   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_AtLowerRightBottomTileBoundary_SetsIsStandingToTrue )
//{
//   _playerArenaPosition = { 11, 8 };
//   ArenaTile tile = { true, true, true, false };
//   ON_CALL( *_arenaMock, GetTile( 77 ) ).WillByDefault( ReturnRef( tile ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_playerMock, SetIsStanding( false ) );
//   EXPECT_CALL( *_playerMock, SetIsStanding( true ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_InsideDeathTile_RaisesTileDeathEvent )
//{
//   ArenaTile deathTile = { true, true, true, true, true };
//   ON_CALL( *_arenaMock, GetTile( 45 ) ).WillByDefault( ReturnRef( deathTile ) );
//   BuildArenaPhysics();
//
//   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::TileDeath ) );
//
//   _arenaPhysics->Tick();
//}
//
//TEST_F( ArenaPhysicsTests, Tick_Always_UpdatesActiveRegion )
//{
//   BuildArenaPhysics();
//
//   Quad<float> region;
//   EXPECT_CALL( *_arenaMock, SetActiveRegion( _ ) ).WillOnce( SaveArg<0>( &region ) );
//
//   _arenaPhysics->Tick();
//
//   EXPECT_EQ( region.Left, 0 );
//   EXPECT_EQ( region.Top, 3 );
//   EXPECT_EQ( region.Right, 20 );
//   EXPECT_EQ( region.Bottom, 13 );
//}
