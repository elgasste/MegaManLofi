#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Game.h>
#include <MegaManLofi/GameState.h>
#include <MegaManLofi/Direction.h>
#include <MegaManLofi/GameCommand.h>
#include <MegaManLofi/GameEvent.h>
#include <MegaManLofi/PushPlayerCommandArgs.h>
#include <MegaManLofi/PointPlayerCommandArgs.h>

#include "mock_GameEventAggregator.h"
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
      _playerPhysicsMock.reset( new NiceMock<mock_PlayerPhysics> );
      _arenaPhysicsMock.reset( new NiceMock<mock_ArenaPhysics> );

      _game.reset( new Game( _eventAggregatorMock, _playerPhysicsMock, _arenaPhysicsMock ) );
   }

protected:
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_PlayerPhysics> _playerPhysicsMock;
   shared_ptr<mock_ArenaPhysics> _arenaPhysicsMock;

   shared_ptr<Game> _game;
};

TEST_F( GameTests, Constructor_Always_SetsGameStateToStartup )
{
   EXPECT_EQ( _game->GetGameState(), GameState::Startup );
}

TEST_F( GameTests, ExecuteCommand_Start_SetsGameStateToPlaying )
{
   _game->ExecuteCommand( GameCommand::Start );

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_Quit_RaisesShutdownEvent )
{
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::Shutdown ) );

   _game->ExecuteCommand( GameCommand::Quit );
}

TEST_F( GameTests, ExecuteCommand_PushPlayer_PushesPlayerInSpecifiedDirection )
{
   EXPECT_CALL( *_playerPhysicsMock, PushTo( Direction::UpLeft ) );

   _game->ExecuteCommand( GameCommand::PushPlayer,
                          shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::UpLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_PointPlayer_PointsPlayerInSpecifiedDirection )
{
   EXPECT_CALL( *_playerPhysicsMock, PointTo( Direction::DownLeft ) );

   _game->ExecuteCommand( GameCommand::PointPlayer,
                          shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_Jump_Jumps )
{
   EXPECT_CALL( *_playerPhysicsMock, Jump() );

   _game->ExecuteCommand( GameCommand::Jump );
}

TEST_F( GameTests, ExecuteCommand_ExtendJump_ExtendsJump )
{
   EXPECT_CALL( *_playerPhysicsMock, ExtendJump() );

   _game->ExecuteCommand( GameCommand::ExtendJump );
}

TEST_F( GameTests, Tick_GameStateIsNotPlaying_DoesNotDoPlayerOrArenaActions )
{
   EXPECT_CALL( *_playerPhysicsMock, Tick() ).Times( 0 );
   EXPECT_CALL( *_arenaPhysicsMock, MovePlayer() ).Times( 0 );

   _game->Tick();
}

TEST_F( GameTests, Tick_GameStateIsPlaying_DoesPlayerAndArenaActions )
{
   _game->ExecuteCommand( GameCommand::Start );

   EXPECT_CALL( *_playerPhysicsMock, Tick() );
   EXPECT_CALL( *_arenaPhysicsMock, MovePlayer() );

   _game->Tick();
}
