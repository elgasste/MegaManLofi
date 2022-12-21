#include "PlayingMenuStateInputHandler.h"
#include "IGameInputReader.h"
#include "IGameCommandExecutor.h"

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
   // TODO
}
