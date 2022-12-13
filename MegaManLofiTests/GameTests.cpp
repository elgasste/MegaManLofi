#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Game.h>
#include <MegaManLofi/GameState.h>
#include <MegaManLofi/Direction.h>
#include <MegaManLofi/GameCommand.h>
#include <MegaManLofi/GameEvent.h>
#include <MegaManLofi/PushPlayerCommandArgs.h>
#include <MegaManLofi/PointPlayerCommandArgs.h>
#include <MegaManLofi/GameEventAggregator.h>

#include "mock_GameEventAggregator.h"
#include "mock_Player.h"
#include "mock_Arena.h"
#include "mock_PlayerPhysics.h"
#include "mock_ArenaPhysics.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameTests : public Test
{
public:
   void SetUp() override
   {
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _playerMock.reset( new NiceMock<mock_Player> );
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _playerPhysicsMock.reset( new NiceMock<mock_PlayerPhysics> );
      _arenaPhysicsMock.reset( new NiceMock<mock_ArenaPhysics> );

      ON_CALL( *_playerMock, GetLivesRemaining() ).WillByDefault( Return( 5 ) );
   }

   void BuildGame()
   {
      _game.reset( new Game( _eventAggregatorMock, _playerMock, _arenaMock, _playerPhysicsMock, _arenaPhysicsMock ) );
   }

protected:
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_PlayerPhysics> _playerPhysicsMock;
   shared_ptr<mock_ArenaPhysics> _arenaPhysicsMock;

   shared_ptr<Game> _game;
};

TEST_F( GameTests, Constructor_Always_SetsGameStateToTitle )
{
   BuildGame();

   EXPECT_EQ( _game->GetGameState(), GameState::Title );
}

TEST_F( GameTests, ExecuteCommand_StartStage_ResetsGameObjects )
{
   BuildGame();

   EXPECT_CALL( *_playerMock, Reset() );
   EXPECT_CALL( *_arenaMock, Reset() );

   _game->ExecuteCommand( GameCommand::StartStage );
}

TEST_F( GameTests, ExecuteCommand_StartStage_AssignsObjectsToPhysics )
{
   BuildGame();

   auto basePlayer = static_pointer_cast<IPlayer>( _playerMock );
   auto baseArena = static_pointer_cast<IArena>( _arenaMock );

   EXPECT_CALL( *_playerPhysicsMock, AssignTo( basePlayer ) );
   EXPECT_CALL( *_arenaPhysicsMock, AssignTo( baseArena, basePlayer ) );

   _game->ExecuteCommand( GameCommand::StartStage );
}

TEST_F( GameTests, ExecuteCommand_StartStage_SetsNextGameStateToPlaying )
{
   BuildGame();

   _game->ExecuteCommand( GameCommand::StartStage );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_StartStage_RaisesStageStartedEvent )
{
   BuildGame();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::StageStarted ) );

   _game->ExecuteCommand( GameCommand::StartStage );
}

TEST_F( GameTests, ExecuteCommand_TogglePauseAndNotInPlayingState_DoesNotTogglePause )
{
   BuildGame();

   EXPECT_FALSE( _game->IsPaused() );

   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_FALSE( _game->IsPaused() );
}

TEST_F( GameTests, ExecuteCommand_TogglePauseAndInPlayingState_TogglesPause )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   EXPECT_FALSE( _game->IsPaused() );

   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_TRUE( _game->IsPaused() );
}

TEST_F( GameTests, ExecuteCommand_Quit_RaisesShutdownEvent )
{
   BuildGame();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Shutdown ) );

   _game->ExecuteCommand( GameCommand::Quit );
}

TEST_F( GameTests, ExecuteCommand_PushPlayerAndGameIsPaused_DoesNotPushPlayer )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_CALL( *_playerPhysicsMock, PushTo( _ ) ).Times( 0 );

   _game->ExecuteCommand( GameCommand::PushPlayer,
                          shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::UpLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_PushPlayerAndGameIsNotPaused_PushesPlayerInSpecifiedDirection )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   EXPECT_CALL( *_playerPhysicsMock, PushTo( Direction::UpLeft ) );

   _game->ExecuteCommand( GameCommand::PushPlayer,
                          shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::UpLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_PointPlayerAndGameIsPaused_DoesNotPointPlayer )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_CALL( *_playerPhysicsMock, PointTo( _ ) ).Times( 0 );

   _game->ExecuteCommand( GameCommand::PointPlayer,
                          shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_PointPlayerAndGameIsNotPaused_PointsPlayerInSpecifiedDirection )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   EXPECT_CALL( *_playerPhysicsMock, PointTo( Direction::DownLeft ) );

   _game->ExecuteCommand( GameCommand::PointPlayer,
                          shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_JumpAndGameIsPaused_DoesNotJump )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_CALL( *_playerPhysicsMock, Jump() ).Times( 0 );

   _game->ExecuteCommand( GameCommand::Jump );
}

TEST_F( GameTests, ExecuteCommand_JumpAndGameIsNotPaused_Jumps )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   EXPECT_CALL( *_playerPhysicsMock, Jump() );

   _game->ExecuteCommand( GameCommand::Jump );
}

TEST_F( GameTests, ExecuteCommand_ExtendJumpAndGameIsPaused_DoesNotExtendJump )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_CALL( *_playerPhysicsMock, ExtendJump() ).Times( 0 );

   _game->ExecuteCommand( GameCommand::ExtendJump );
}

TEST_F( GameTests, ExecuteCommand_ExtendJumpAndGameIsNotPaused_ExtendsJump )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   EXPECT_CALL( *_playerPhysicsMock, ExtendJump() );

   _game->ExecuteCommand( GameCommand::ExtendJump );
}

TEST_F( GameTests, Tick_GameStateIsNotPlaying_DoesNotDoPlayerOrArenaActions )
{
   BuildGame();

   EXPECT_CALL( *_playerPhysicsMock, Tick() ).Times( 0 );
   EXPECT_CALL( *_arenaPhysicsMock, Tick() ).Times( 0 );

   _game->Tick();
}

TEST_F( GameTests, Tick_GameIsPaused_DoesNotDoPlayerOrArenaActions )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_CALL( *_playerPhysicsMock, Tick() ).Times( 0 );
   EXPECT_CALL( *_arenaPhysicsMock, Tick() ).Times( 0 );

   _game->Tick();
}

TEST_F( GameTests, Tick_GameStateIsPlayingAndNotPaused_DoesPlayerAndArenaActions )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   EXPECT_CALL( *_playerPhysicsMock, Tick() );
   EXPECT_CALL( *_arenaPhysicsMock, Tick() );

   _game->Tick();
}

TEST_F( GameTests, EventHandling_PitfallEventRaisedWithLivesLeft_DecrementsPlayerLivesRemaining )
{
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _arenaMock, _playerPhysicsMock, _arenaPhysicsMock ) );

   EXPECT_CALL( *_playerMock, SetLivesRemaining( 4 ) );

   eventAggregator->RaiseEvent( GameEvent::Pitfall );
   _game->Tick();
}

TEST_F( GameTests, EventHandling_PitfallEventRaisedWithNoLivesLeft_ChangesNextGameStateToGameOver )
{
   ON_CALL( *_playerMock, GetLivesRemaining() ).WillByDefault( Return( 0 ) );
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _arenaMock, _playerPhysicsMock, _arenaPhysicsMock ) );

   eventAggregator->RaiseEvent( GameEvent::Pitfall );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::GameOver );
}

TEST_F( GameTests, EventHandling_TileDeathEventRaisedWithLivesLeft_DecrementsPlayerLivesRemaining )
{
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _arenaMock, _playerPhysicsMock, _arenaPhysicsMock ) );

   EXPECT_CALL( *_playerMock, SetLivesRemaining( 4 ) );

   eventAggregator->RaiseEvent( GameEvent::TileDeath );
   _game->Tick();
}

TEST_F( GameTests, EventHandling_TileDeathEventRaised_ChangesNextGameStateToGameOver )
{
   ON_CALL( *_playerMock, GetLivesRemaining() ).WillByDefault( Return( 0 ) );
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _arenaMock, _playerPhysicsMock, _arenaPhysicsMock ) );

   eventAggregator->RaiseEvent( GameEvent::TileDeath );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::GameOver );
}
