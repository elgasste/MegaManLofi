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
   _state( GameState::Startup ),
   _nextState( GameState::Startup )
{
   _eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &Game::HandlePitfallEvent, this ) );
   _eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &Game::HandleTileDeathEvent, this ) );
}

void Game::Tick()
{
   _state = _nextState;

   if ( _state == GameState::Playing )
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
   switch ( command )
   {
      case GameCommand::Start:
         _player->Reset();
         _arena->Reset();
         _playerPhysics->AssignTo( _player );
         _arenaPhysics->AssignTo( _arena, _player );
         _nextState = GameState::Playing;
         _eventAggregator->RaiseEvent( GameEvent::GameStarted );
         break;
      case GameCommand::Quit:
         _eventAggregator->RaiseEvent( GameEvent::Shutdown );
         break;
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

void Game::HandlePitfallEvent()
{
   _nextState = GameState::GameOver;
}

void Game::HandleTileDeathEvent()
{
   _nextState = GameState::GameOver;
}
