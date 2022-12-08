#include "Game.h"
#include "IGameEventAggregator.h"
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
   _arena( arena ),
   _playerPhysics( playerPhysics ),
   _arenaPhysics( arenaPhysics ),
   _state( GameState::Startup )
{
   _playerPhysics->AssignTo( player );
   _arenaPhysics->AssignTo( arena, player );

   _eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &Game::HandlePitfallEvent, this ) );
}

void Game::Tick()
{
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
         _arena->Reset();
         _state = GameState::Playing;
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
   _state = GameState::GameOver;
}
