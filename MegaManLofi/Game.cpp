#include "Game.h"
#include "IGameEventAggregator.h"
#include "IPlayer.h"
#include "IArena.h"
#include "IPlayerPhysics.h"
#include "IArenaPhysics.h"
#include "GameState.h"
#include "GameCommand.h"
#include "GameEvent.h"
#include "PushPlayerCommandArgs.h"
#include "PointPlayerCommandArgs.h"

using namespace std;
using namespace MegaManLofi;

Game::Game( const shared_ptr<IGameEventAggregator> eventAggregator,
            const shared_ptr<IPlayer> player,
            const shared_ptr<IArena> arena,
            const shared_ptr<IPlayerPhysics> playerPhysics,
            const shared_ptr<IArenaPhysics> arenaPhysics ) :
   _eventAggregator( eventAggregator ),
   _player( player ),
   _arena( arena ),
   _playerPhysics( playerPhysics ),
   _arenaPhysics( arenaPhysics ),
   _state( GameState::Title ),
   _nextState( GameState::Title ),
   _isPaused( false ),
   _restartStageNextFrame( false )
{
   _eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &Game::KillPlayer, this ) );
   _eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &Game::KillPlayer, this ) );
}

void Game::Tick()
{
   if ( _restartStageNextFrame )
   {
      _player->ResetPhysics();
      _arena->Reset();
      _arenaPhysics->AssignTo( _arena, _player );
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

void Game::ExecuteCommand( GameCommand command )
{
   ExecuteCommand( command, nullptr );
}

void Game::ExecuteCommand( GameCommand command, const shared_ptr<GameCommandArgs> args )
{
   // commands that don't observe _isPaused
   switch ( command )
   {
      case GameCommand::StartStage:
         _player->Reset();
         _arena->Reset();
         _playerPhysics->AssignTo( _player );
         _arenaPhysics->AssignTo( _arena, _player );
         _nextState = GameState::Playing;
         _isPaused = false;
         _eventAggregator->RaiseEvent( GameEvent::StageStarted );
         break;
      case GameCommand::TogglePause:
         if ( _nextState == GameState::Playing )
         {
            _isPaused = !_isPaused;
         }
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
