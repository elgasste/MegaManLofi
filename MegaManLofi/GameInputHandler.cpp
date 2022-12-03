#include "GameInputHandler.h"
#include "IGameInputReader.h"
#include "IGameInfoProvider.h"
#include "IGameEventAggregator.h"
#include "GameButton.h"
#include "GameEvent.h"
#include "GameState.h"

using namespace std;
using namespace MegaManLofi;

GameInputHandler::GameInputHandler( const shared_ptr<IGameInputReader> inputReader,
                                    const shared_ptr<IGameInfoProvider> gameInfoProvider,
                                    const shared_ptr<IGameEventAggregator> eventAggregator ) :
   _inputReader( inputReader ),
   _gameInfoProvider( gameInfoProvider ),
   _eventAggregator( eventAggregator )
{
}

void GameInputHandler::AddInputHandlerForGameState( GameState state,
                                                    shared_ptr<IGameInputHandler> inputHandler )
{
   _stateInputHandlers[state] = inputHandler;
}

void GameInputHandler::HandleInput()
{
   _inputReader->ReadInput();

   if ( _inputReader->WasButtonPressed( GameButton::Diagnostics ) )
   {
      _eventAggregator->RaiseEvent( GameEvent::ToggleDiagnostics );
   }

   _stateInputHandlers.at( _gameInfoProvider->GetGameState() )->HandleInput();
}
