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

TEST_F( GameTests, Constructor_Always_SetsGameStateToStartup )
{
   BuildGame();

   EXPECT_EQ( _game->GetGameState(), GameState::Startup );
}

TEST_F( GameTests, Constructor_Always_AssignsPhysicsObjects )
{
   auto basePlayer = static_pointer_cast<IPlayer>( _playerMock );
   auto baseArena = static_pointer_cast<IArena>( _arenaMock );

   EXPECT_CALL( *_playerPhysicsMock, AssignTo( basePlayer ) );
   EXPECT_CALL( *_arenaPhysicsMock, AssignTo( baseArena, basePlayer ) );

   BuildGame();
}

TEST_F( GameTests, ExecuteCommand_Start_ResetsGameObjects )
{
   BuildGame();

   EXPECT_CALL( *_playerMock, Reset() );
   EXPECT_CALL( *_arenaMock, Reset() );

   _game->ExecuteCommand( GameCommand::Start );
}

TEST_F( GameTests, ExecuteCommand_Start_SetsGameStateToPlaying )
{
   BuildGame();

   _game->ExecuteCommand( GameCommand::Start );

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_Start_RaisesGameStartedEvent )
{
   BuildGame();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::GameStarted ) );

   _game->ExecuteCommand( GameCommand::Start );
}

TEST_F( GameTests, ExecuteCommand_Quit_RaisesShutdownEvent )
{
   BuildGame();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Shutdown ) );

   _game->ExecuteCommand( GameCommand::Quit );
}

TEST_F( GameTests, ExecuteCommand_PushPlayer_PushesPlayerInSpecifiedDirection )
{
   BuildGame();

   EXPECT_CALL( *_playerPhysicsMock, PushTo( Direction::UpLeft ) );

   _game->ExecuteCommand( GameCommand::PushPlayer,
                          shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::UpLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_PointPlayer_PointsPlayerInSpecifiedDirection )
{
   BuildGame();

   EXPECT_CALL( *_playerPhysicsMock, PointTo( Direction::DownLeft ) );

   _game->ExecuteCommand( GameCommand::PointPlayer,
                          shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_Jump_Jumps )
{
   BuildGame();

   EXPECT_CALL( *_playerPhysicsMock, Jump() );

   _game->ExecuteCommand( GameCommand::Jump );
}

TEST_F( GameTests, ExecuteCommand_ExtendJump_ExtendsJump )
{
   BuildGame();

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

TEST_F( GameTests, Tick_GameStateIsPlaying_DoesPlayerAndArenaActions )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::Start );

   EXPECT_CALL( *_playerPhysicsMock, Tick() );
   EXPECT_CALL( *_arenaPhysicsMock, Tick() );

   _game->Tick();
}

TEST_F( GameTests, EventHandling_PitfallEventRaised_ChangesGameStateToGameOver )
{
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _arenaMock, _playerPhysicsMock, _arenaPhysicsMock ) );

   eventAggregator->RaiseEvent( GameEvent::Pitfall );

   EXPECT_EQ( _game->GetGameState(), GameState::GameOver );
}
