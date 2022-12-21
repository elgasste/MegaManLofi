#include "PlayingMenuStateInputHandler.h"
#include "IGameInputReader.h"
#include "IGameCommandExecutor.h"
#include "GameButton.h"
#include "GameCommand.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenuStateInputHandler::PlayingMenuStateInputHandler( const shared_ptr<IGameInputReader> inputReader,
                                                            const shared_ptr<IGameCommandExecutor> commandExecutor ) :
   _inputReader( inputReader ),
   _commandExecutor( commandExecutor )
{
}

void PlayingMenuStateInputHandler::HandleInput()
{
   if ( _inputReader->WasButtonPressed( GameButton::Start ) )
   {
      _commandExecutor->ExecuteCommand( GameCommand::ClosePlayingMenu );
   }

   if ( _inputReader->WasButtonPressed( GameButton::Select ) )
   {
      _commandExecutor->ExecuteCommand( GameCommand::Quit );
   }
}
