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
#include <MegaManLofi/EntityDefs.h>

#include "mock_GameEventAggregator.h"
#include "mock_Player.h"
#include "mock_Stage.h"
#include "mock_Arena.h"
#include "mock_PlayerPhysics.h"
#include "mock_ArenaPhysics.h"
#include "mock_EntityFactory.h"
#include "mock_Entity.h"

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
      _stageMock.reset( new NiceMock<mock_Stage> );
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _playerPhysicsMock.reset( new NiceMock<mock_PlayerPhysics> );
      _arenaPhysicsMock.reset( new NiceMock<mock_ArenaPhysics> );
      _entityFactoryMock.reset( new NiceMock<mock_EntityFactory> );
      _entityDefs.reset( new EntityDefs );

      _entityDefs->BulletEntityMetaId = 12;

      _playerHitBox = { 0, 0, 6, 10 };
      ON_CALL( *_playerMock, GetArenaPositionLeft() ).WillByDefault( Return( 20.0f ) );
      ON_CALL( *_playerMock, GetArenaPositionTop() ).WillByDefault( Return( 10.0f ) );
      ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( _playerHitBox ) );
      ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::Right ) );
      ON_CALL( *_playerMock, GetLivesRemaining() ).WillByDefault( Return( 5 ) );

      ON_CALL( *_stageMock, GetActiveArena() ).WillByDefault( Return( _arenaMock ) );
      ON_CALL( *_stageMock, GetMutableActiveArena() ).WillByDefault( Return( _arenaMock ) );
   }

   void BuildGame()
   {
      _game.reset( new Game( _eventAggregatorMock, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );
   }

protected:
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_Player> _playerMock;
   shared_ptr<mock_Stage> _stageMock;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_PlayerPhysics> _playerPhysicsMock;
   shared_ptr<mock_ArenaPhysics> _arenaPhysicsMock;
   shared_ptr<mock_EntityFactory> _entityFactoryMock;
   shared_ptr<EntityDefs> _entityDefs;

   Rectangle<float> _playerHitBox;

   shared_ptr<Game> _game;
};

TEST_F( GameTests, Constructor_Always_SetsGameStateToTitle )
{
   BuildGame();

   EXPECT_EQ( _game->GetGameState(), GameState::Title );
}

TEST_F( GameTests, ExecuteCommand_StartGame_ResetsGameObjects )
{
   BuildGame();

   EXPECT_CALL( *_playerMock, Reset() );
   EXPECT_CALL( *_playerPhysicsMock, AssignTo( static_pointer_cast<Player>( _playerMock ) ) );
   EXPECT_CALL( *_stageMock, Reset() );
   EXPECT_CALL( *_arenaPhysicsMock, AssignTo( static_pointer_cast<Stage>( _stageMock ) ) );
   EXPECT_CALL( *_arenaMock, SetPlayerEntity( static_pointer_cast<Entity>( _playerMock ) ) );
   EXPECT_CALL( *_playerMock, ResetPosition() );
   EXPECT_CALL( *_arenaMock, Reset() );
   EXPECT_CALL( *_arenaPhysicsMock, Reset() );
   EXPECT_CALL( *_playerPhysicsMock, Reset() );

   _game->ExecuteCommand( GameCommand::StartGame );
}

TEST_F( GameTests, ExecuteCommand_StartGame_SetsNextGameStateToPlaying )
{
   BuildGame();

   _game->ExecuteCommand( GameCommand::StartGame );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_StartGame_RaisesGameStartedAndStageStartedEvents )
{
   BuildGame();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::GameStarted ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::StageStarted ) );

   _game->ExecuteCommand( GameCommand::StartGame );
}

TEST_F( GameTests, ExecuteCommand_StartStage_ResetsGameObjects )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartGame );

   EXPECT_CALL( *_playerMock, ResetPosition() );
   EXPECT_CALL( *_arenaMock, Reset() );
   EXPECT_CALL( *_arenaPhysicsMock, Reset() );
   EXPECT_CALL( *_playerPhysicsMock, Reset() );

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

TEST_F( GameTests, ExecuteCommand_OpenPlayingMenuAndNotInPlayingState_DoesNotChangeGameState )
{
   BuildGame();

   EXPECT_EQ( _game->GetGameState(), GameState::Title );

   _game->ExecuteCommand( GameCommand::OpenPlayingMenu );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Title );
}

TEST_F( GameTests, ExecuteCommand_OpenPlayingMenuAndInPlayingStateAndPaused_DoesNotChangeState )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );

   _game->ExecuteCommand( GameCommand::OpenPlayingMenu );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_OpenPlayingMenuAndInPlayingStateAndNotPaused_SetsNextStateToPlayingMenu )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );

   _game->ExecuteCommand( GameCommand::OpenPlayingMenu );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::PlayingMenu );
}

TEST_F( GameTests, ExecuteCommand_ClosePlayingMenuAndPlayingMenuIsNotOpen_DoesNotChangeState )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );

   _game->ExecuteCommand( GameCommand::ClosePlayingMenu );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_ClosePlayingMenuAndPlayingMenuIsOpen_SetsNextStateToPlaying )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::OpenPlayingMenu );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::PlayingMenu );

   _game->ExecuteCommand( GameCommand::ClosePlayingMenu );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Playing );
}

TEST_F( GameTests, ExecuteCommand_ExitToTitle_SetsNextStateToExitToTitle )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::ExitToTitle );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::Title );
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

TEST_F( GameTests, ExecuteCommand_ShootAndNextStateIsNotPlaying_DoesNotAddBulletToArena )
{
   BuildGame();

   EXPECT_CALL( *_arenaMock, AddEntity( _ ) ).Times( 0 );

   _game->ExecuteCommand( GameCommand::Shoot );
}

TEST_F( GameTests, ExecuteCommand_ShootAndGameIsPaused_DoesNotAddBulletToArena )
{
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );
   _game->ExecuteCommand( GameCommand::TogglePause );

   EXPECT_CALL( *_arenaMock, AddEntity( _ ) ).Times( 0 );

   _game->ExecuteCommand( GameCommand::Shoot );
}

TEST_F( GameTests, ExecuteCommand_ShootLeft_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::Left ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::Left ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 20 );
   EXPECT_EQ( bulletPosition.Top, 15 );
}

TEST_F( GameTests, ExecuteCommand_ShootUpLeft_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::UpLeft ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::UpLeft ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 20 );
   EXPECT_EQ( bulletPosition.Top, 10 );
}

TEST_F( GameTests, ExecuteCommand_ShootUp_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::Up ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::Up ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 23 );
   EXPECT_EQ( bulletPosition.Top, 10 );
}

TEST_F( GameTests, ExecuteCommand_ShootUpRight_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::UpRight ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::UpRight ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 26 );
   EXPECT_EQ( bulletPosition.Top, 10 );
}

TEST_F( GameTests, ExecuteCommand_ShootRight_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::Right ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::Right ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 26 );
   EXPECT_EQ( bulletPosition.Top, 15 );
}

TEST_F( GameTests, ExecuteCommand_ShootDownRight_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::DownRight ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::DownRight ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 26 );
   EXPECT_EQ( bulletPosition.Top, 20 );
}

TEST_F( GameTests, ExecuteCommand_ShootDown_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::Down ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::Down ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 23 );
   EXPECT_EQ( bulletPosition.Top, 20 );
}

TEST_F( GameTests, ExecuteCommand_ShootDownLeft_AddsBulletToArenaInCorrectPosition )
{
   ON_CALL( *_playerMock, GetDirection() ).WillByDefault( Return( Direction::DownLeft ) );
   BuildGame();
   _game->ExecuteCommand( GameCommand::StartStage );

   auto bulletMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 12, Direction::DownLeft ) ).WillOnce( Return( bulletMock ) );

   Coordinate<float> bulletPosition;
   EXPECT_CALL( *bulletMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &bulletPosition ) );

   _game->ExecuteCommand( GameCommand::Shoot );

   EXPECT_EQ( bulletPosition.Left, 20 );
   EXPECT_EQ( bulletPosition.Top, 20 );
}

TEST_F( GameTests, Tick_RestartingStageNextFrame_ResetsGameObjects )
{
   EXPECT_CALL( *_playerMock, SetLivesRemaining( 4 ) );

   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );
   eventAggregator->RaiseEvent( GameEvent::TileDeath );

   EXPECT_CALL( *_playerMock, ResetPosition() );
   EXPECT_CALL( *_arenaMock, Reset() );
   EXPECT_CALL( *_arenaPhysicsMock, Reset() );
   EXPECT_CALL( *_playerPhysicsMock, Reset() );

   _game->Tick();
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
   EXPECT_CALL( *_arenaMock, DeSpawnInactiveEntities() );
   EXPECT_CALL( *_arenaMock, CheckSpawnPoints() );

   _game->Tick();
}

TEST_F( GameTests, EventHandling_PitfallEventRaisedWithLivesLeft_DecrementsPlayerLivesRemaining )
{
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );

   EXPECT_CALL( *_playerMock, SetLivesRemaining( 4 ) );

   eventAggregator->RaiseEvent( GameEvent::Pitfall );
   _game->Tick();
}

TEST_F( GameTests, EventHandling_PitfallEventRaisedWithNoLivesLeft_ChangesNextGameStateToGameOver )
{
   ON_CALL( *_playerMock, GetLivesRemaining() ).WillByDefault( Return( 0 ) );
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );

   eventAggregator->RaiseEvent( GameEvent::Pitfall );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::GameOver );
}

TEST_F( GameTests, EventHandling_TileDeathEventRaisedWithLivesLeft_DecrementsPlayerLivesRemaining )
{
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );

   EXPECT_CALL( *_playerMock, SetLivesRemaining( 4 ) );

   eventAggregator->RaiseEvent( GameEvent::TileDeath );
   _game->Tick();
}

TEST_F( GameTests, EventHandling_TileDeathEventRaised_ChangesNextGameStateToGameOver )
{
   ON_CALL( *_playerMock, GetLivesRemaining() ).WillByDefault( Return( 0 ) );
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );

   eventAggregator->RaiseEvent( GameEvent::TileDeath );
   _game->Tick();

   EXPECT_EQ( _game->GetGameState(), GameState::GameOver );
}

TEST_F( GameTests, EventHandling_ActiveArenaChangedEventRaised_AssignsPlayerToArena )
{
   auto eventAggregator = make_shared<GameEventAggregator>();
   _game.reset( new Game( eventAggregator, _playerMock, _stageMock, _playerPhysicsMock, _arenaPhysicsMock, _entityFactoryMock, _entityDefs ) );

   EXPECT_CALL( *_arenaMock, SetPlayerEntity( static_pointer_cast<Entity>( _playerMock ) ) );

   eventAggregator->RaiseEvent( GameEvent::ActiveArenaChanged );
}

TEST_F( GameTests, GetPlayer_Always_ReturnsPlayer )
{
   BuildGame();

   EXPECT_EQ( _game->GetPlayer(), _playerMock );
}

TEST_F( GameTests, GetPlayerEntity_Always_ReturnsPlayerAsEntity )
{
   BuildGame();

   EXPECT_EQ( _game->GetPlayerEntity(), _playerMock );
}

TEST_F( GameTests, GetActiveArena_Always_ReturnsActiveArena )
{
   BuildGame();

   EXPECT_EQ( _game->GetActiveArena(), _arenaMock );
}
