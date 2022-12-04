#include "Game.h"
#include "GameConfig.h"
#include "IGameEventAggregator.h"
#include "IPlayerPhysics.h"
#include "IArenaPhysics.h"
#include "IPlayer.h"
#include "IArena.h"
#include "GameState.h"
#include "GameCommand.h"
#include "GameEvent.h"
#include "Direction.h"
#include "PushPlayerCommandArgs.h"
#include "PointPlayerCommandArgs.h"

using namespace std;
using namespace MegaManLofi;

Game::Game( const std::shared_ptr<GameConfig> config,
            const std::shared_ptr<IGameEventAggregator> eventAggregator,
            const std::shared_ptr<IPlayerPhysics> playerPhysics,
            const std::shared_ptr<IArenaPhysics> arenaPhysics,
            const std::shared_ptr<IPlayer> player,
            const std::shared_ptr<IArena> arena ) :
   _config( config ),
   _eventAggregator( eventAggregator ),
   _playerPhysics( playerPhysics ),
   _arenaPhysics( arenaPhysics ),
   _player( player ),
   _arena( arena ),
   _state( GameState::Startup )
{
}

void Game::RunFrame()
{
   if ( _state == GameState::Playing )
   {
      _playerPhysics->ApplyFriction();
      _playerPhysics->ApplyGravity();

      _arenaPhysics->MovePlayer();
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
         _state = GameState::Playing;
         break;
      case GameCommand::Quit:
         _eventAggregator->RaiseEvent( GameEvent::Shutdown );
         break;
      case GameCommand::PushPlayer:
         _playerPhysics->Push( static_pointer_cast<PushPlayerCommandArgs>( args )->Direction );
         break;
      case GameCommand::PointPlayer:
         _player->Point( static_pointer_cast<PointPlayerCommandArgs>( args )->Direction );
         break;
      case GameCommand::Jump:
         _playerPhysics->Jump();
         break;
   }
}
