#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Game.h>
#include <MegaManLofi/GameConfig.h>
#include <MegaManLofi/GameState.h>
#include <MegaManLofi/Direction.h>
#include <MegaManLofi/GameCommand.h>
#include <MegaManLofi/GameEvent.h>
#include <MegaManLofi/PushPlayerCommandArgs.h>
#include <MegaManLofi/PointPlayerCommandArgs.h>

#include "mock_GameEventAggregator.h"
#include "mock_Physics.h"
#include "mock_Player.h"
#include "mock_Arena.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameTests : public Test
{
public:
   void SetUp() override
   {
      _config.reset( new GameConfig );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _physicsMock.reset( new NiceMock<mock_Physics> );
      _playerMock.reset( new NiceMock<mock_Player> );
      _arenaMock.reset( new NiceMock<mock_Arena> );

      _game.reset( new Game( _config, _eventAggregatorMock, _physicsMock, _playerMock, _arenaMock ) );
   }

protected:
   shared_ptr<GameConfig> _config;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_Physics> _physicsMock;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<mock_Arena> _arenaMock;

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
   EXPECT_CALL( *_physicsMock, PlayerPush( _, Direction::UpLeft ) );

   _game->ExecuteCommand( GameCommand::PushPlayer,
                          shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::UpLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_PointPlayer_PointsPlayerInSpecifiedDirection )
{
   EXPECT_CALL( *_playerMock, Point( Direction::DownLeft ) );

   _game->ExecuteCommand( GameCommand::PointPlayer,
                          shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownLeft ) ) );
}

TEST_F( GameTests, ExecuteCommand_Jump_Jumps )
{
   EXPECT_CALL( *_physicsMock, PlayerJump( _ ) );

   _game->ExecuteCommand( GameCommand::Jump );
}

TEST_F( GameTests, RunFrame_GameStateIsNotPlaying_DoesNotDoPlayerOrArenaActions )
{
   EXPECT_CALL( *_physicsMock, PlayerApplyFriction( _ ) ).Times( 0 );
   EXPECT_CALL( *_physicsMock, PlayerApplyGravity( _ ) ).Times( 0 );
   EXPECT_CALL( *_arenaMock, MovePlayer() ).Times( 0 );

   _game->RunFrame();
}

TEST_F( GameTests, RunFrame_GameStateIsPlaying_DoesPlayerAndArenaActions )
{
   _game->ExecuteCommand( GameCommand::Start );

   EXPECT_CALL( *_physicsMock, PlayerApplyFriction( _ ) );
   EXPECT_CALL( *_physicsMock, PlayerApplyGravity( _ ) );
   EXPECT_CALL( *_arenaMock, MovePlayer() );

   _game->RunFrame();
}
