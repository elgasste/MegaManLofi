#include "TitleStateInputHandler.h"
#include "IGameInputReader.h"
#include "IGameCommandExecutor.h"
#include "GameCommand.h"

using namespace std;
using namespace MegaManLofi;

TitleStateInputHandler::TitleStateInputHandler( const shared_ptr<IGameInputReader> inputReader,
                                                const shared_ptr<IGameCommandExecutor> commandExecutor ) :
   _inputReader( inputReader ),
   _commandExecutor( commandExecutor )
{
}

void TitleStateInputHandler::HandleInput()
{
   if ( _inputReader->WasAnyButtonPressed() )
   {
      _commandExecutor->ExecuteCommand( GameCommand::Start );
   }
}
