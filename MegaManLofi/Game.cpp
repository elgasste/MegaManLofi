#include "Game.h"
#include "GameEventAggregator.h"
#include "Player.h"
#include "Stage.h"
#include "Arena.h"
#include "EntityPhysics.h"
#include "ArenaPhysics.h"
#include "EntityFactory.h"
#include "EntityDefs.h"
#include "GameState.h"
#include "GameCommand.h"
#include "PushPlayerCommandArgs.h"
#include "PointPlayerCommandArgs.h"

using namespace std;
using namespace MegaManLofi;

Game::Game( const shared_ptr<GameEventAggregator> eventAggregator,
            const shared_ptr<Player> player,
            const shared_ptr<Stage> stage,
            const shared_ptr<EntityPhysics> entityPhysics,
            const shared_ptr<ArenaPhysics> arenaPhysics,
            const shared_ptr<EntityFactory> entityFactory,
            const shared_ptr<EntityDefs> entityDefs ) :
   _eventAggregator( eventAggregator ),
   _player( player ),
   _stage( stage ),
   _entityPhysics( entityPhysics ),
   _arenaPhysics( arenaPhysics ),
   _entityFactory( entityFactory ),
   _entityDefs( entityDefs ),
   _state( GameState::Title ),
   _nextState( GameState::Title ),
   _isPaused( false ),
   _restartStageNextFrame( false )
{
   _eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &Game::HandleEnvironmentDeath, this ) );
   _eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &Game::HandleEnvironmentDeath, this ) );
   _eventAggregator->RegisterEventHandler( GameEvent::CollisionDeath, std::bind( &Game::HandleCollisionDeath, this ) );
   _eventAggregator->RegisterEventHandler( GameEvent::ActiveArenaChanged, std::bind( &Game::HandleActiveArenaChanged, this ) );
}

void Game::Tick()
{
   _state = _nextState;

   if ( _restartStageNextFrame )
   {
      _restartStageNextFrame = false;
      StartStage();
   }
   else if ( _state == GameState::Playing && !_isPaused )
   {
      _entityPhysics->Tick();
      _arenaPhysics->Tick();

      auto arena = _stage->GetMutableActiveArena();
      arena->DetectEntityCollisions();
      arena->DeSpawnInactiveEntities();
      arena->CheckSpawnPoints();
   }
}

const shared_ptr<ReadOnlyPlayer> Game::GetPlayer() const
{
   return _player;
}

const shared_ptr<ReadOnlyEntity> Game::GetPlayerEntity() const
{
   return _player;
}

const shared_ptr<ReadOnlyArena> Game::GetActiveArena() const
{
   return _stage->GetActiveArena();
}

void Game::ExecuteCommand( GameCommand command )
{
   ExecuteCommand( command, nullptr );
}

void Game::ExecuteCommand( GameCommand command, const shared_ptr<GameCommandArgs> args )
{
   // commands that don't observe _isPaused
   switch ( command )
   {
      case GameCommand::StartGame:
         StartGame();
         break;
      case GameCommand::StartStage:
         StartStage();
         break;
      case GameCommand::TogglePause:
         TogglePause();
         break;
      case GameCommand::ExitToTitle:
         _nextState = GameState::Title;
         break;
      case GameCommand::Quit:
         _eventAggregator->RaiseEvent( GameEvent::Shutdown );
         break;
   }

   if ( _isPaused )
   {
      return;
   }

   // commands that observe _isPaused
   switch ( command )
   {
      case GameCommand::PushPlayer:
         _player->PushTo( static_pointer_cast<PushPlayerCommandArgs>( args )->Direction );
         break;
      case GameCommand::PointPlayer:
         _player->PointTo( static_pointer_cast<PointPlayerCommandArgs>( args )->Direction );
         break;
      case GameCommand::Jump:
         _player->Jump();
         break;
      case GameCommand::ExtendJump:
         _player->ExtendJump();
         break;
      case GameCommand::Shoot:
         Shoot();
         break;
      case GameCommand::OpenPlayingMenu:
         OpenPlayingMenu();
         break;
      case GameCommand::ClosePlayingMenu:
         ClosePlayingMenu();
         break;
   }
}

void Game::StartGame()
{
   _player->Reset();

   _stage->Reload();
   _entityPhysics->AssignTo( _stage );
   _arenaPhysics->AssignTo( _stage );

   StartStage();
   _eventAggregator->RaiseEvent( GameEvent::GameStarted );
}

void Game::StartStage()
{
   _player->ResetPosition();
   _player->ResetHealth();

   auto arena = _stage->GetMutableActiveArena();
   arena->Reset();
   arena->SetPlayerEntity( _player );

   _arenaPhysics->Reset();

   _nextState = GameState::Playing;
   _isPaused = false;
   _eventAggregator->RaiseEvent( GameEvent::StageStarted );
}

void Game::Shoot()
{
   if ( _nextState != GameState::Playing )
   {
      return;
   }

   auto left = _player->GetArenaPositionLeft();
   auto top = _player->GetArenaPositionTop();
   const auto& hitBox = _player->GetHitBox();
   auto direction = _player->GetDirection();

   left +=
      ( direction == Direction::Up || direction == Direction::Down ) ? hitBox.Width / 2 :
      ( direction == Direction::UpRight || direction == Direction::Right || direction == Direction::DownRight ) ? hitBox.Width : 0;
   top +=
      ( direction == Direction::Left || direction == Direction::Right ) ? hitBox.Height / 2 :
      ( direction == Direction::DownLeft || direction == Direction::Down || direction == Direction::DownRight ) ? hitBox.Height : 0;

   auto bullet = _entityFactory->CreateEntity( _entityDefs->BulletEntityMetaId, _player->GetDirection() );
   bullet->SetArenaPosition( { left, top } );

   _stage->GetMutableActiveArena()->AddEntity( bullet );
}

void Game::TogglePause()
{
   if ( _nextState == GameState::Playing )
   {
      _isPaused = !_isPaused;
   }
}

void Game::OpenPlayingMenu()
{
   if ( _nextState == GameState::Playing && !_isPaused )
   {
      _nextState = GameState::PlayingMenu;
   }
}

void Game::ClosePlayingMenu()
{
   if ( _nextState == GameState::PlayingMenu )
   {
      _nextState = GameState::Playing;
   }
}

void Game::HandleEnvironmentDeath()
{
   _player->SetHealth( 0 );
   _player->SetLivesRemaining( _player->GetLivesRemaining() - 1 );
   HandleCollisionDeath();
}

void Game::HandleCollisionDeath()
{
   if ( _player->GetLivesRemaining() > 0 )
   {
      _restartStageNextFrame = true;
   }
   else
   {
      _nextState = GameState::GameOver;
   }
}

void Game::HandleActiveArenaChanged()
{
   _stage->GetMutableActiveArena()->SetPlayerEntity( _player );
}
