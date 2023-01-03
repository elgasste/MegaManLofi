#include "Game.h"
#include "IGameEventAggregator.h"
#include "Player.h"
#include "Arena.h"
#include "IPlayerPhysics.h"
#include "ArenaPhysics.h"
#include "IEntityFactory.h"
#include "GameState.h"
#include "GameCommand.h"
#include "PushPlayerCommandArgs.h"
#include "PointPlayerCommandArgs.h"

using namespace std;
using namespace MegaManLofi;

Game::Game( const shared_ptr<IGameEventAggregator> eventAggregator,
            const shared_ptr<Player> player,
            const shared_ptr<Arena> arena,
            const shared_ptr<IPlayerPhysics> playerPhysics,
            const shared_ptr<ArenaPhysics> arenaPhysics,
            const shared_ptr<IEntityFactory> entityFactory ) :
   _eventAggregator( eventAggregator ),
   _player( player ),
   _arena( arena ),
   _playerPhysics( playerPhysics ),
   _arenaPhysics( arenaPhysics ),
   _entityFactory( entityFactory ),
   _state( GameState::Title ),
   _nextState( GameState::Title ),
   _isPaused( false ),
   _restartStageNextFrame( false )
{
   _eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &Game::KillPlayer, this ) );
   _eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &Game::KillPlayer, this ) );

   _arena->SetPlayerEntity( _player );
}

void Game::Tick()
{
   if ( _restartStageNextFrame )
   {
      _player->ResetPhysics();
      _arena->Reset();
      _arenaPhysics->AssignTo( _arena );
      _restartStageNextFrame = false;
      _eventAggregator->RaiseEvent( GameEvent::StageStarted );
   }

   _state = _nextState;

   if ( _state == GameState::Playing && !_isPaused )
   {
      _playerPhysics->Tick();
      _arenaPhysics->Tick();
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

const shared_ptr<ReadOnlyArena> Game::GetArena() const
{
   return _arena;
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
         StartStage();
         _eventAggregator->RaiseEvent( GameEvent::GameStarted );
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
         _playerPhysics->PushTo( static_pointer_cast<PushPlayerCommandArgs>( args )->Direction );
         break;
      case GameCommand::PointPlayer:
         _playerPhysics->PointTo( static_pointer_cast<PointPlayerCommandArgs>( args )->Direction );
         break;
      case GameCommand::Jump:
         _playerPhysics->Jump();
         break;
      case GameCommand::ExtendJump:
         _playerPhysics->ExtendJump();
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

void Game::StartStage()
{
   _player->Reset();
   _arena->Reset();
   _playerPhysics->AssignTo( _player );
   _arenaPhysics->AssignTo( _arena );
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

   auto bullet = _entityFactory->CreateBullet( { left, top }, _player->GetDirection() );

   _arena->AddEntity( bullet );
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

void Game::KillPlayer()
{
   _player->SetLivesRemaining( _player->GetLivesRemaining() - 1 );

   if ( _player->GetLivesRemaining() > 0 )
   {
      _restartStageNextFrame = true;
   }
   else
   {
      _nextState = GameState::GameOver;
   }
}
