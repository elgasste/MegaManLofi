#include "PlayingMenuStateInputHandler.h"
#include "IGameInputReader.h"
#include "IGameCommandExecutor.h"
#include "IMenuProvider.h"
#include "IMenu.h"
#include "MenuType.h"
#include "GameButton.h"
#include "GameCommand.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenuStateInputHandler::PlayingMenuStateInputHandler( const shared_ptr<IGameInputReader> inputReader,
                                                            const shared_ptr<IGameCommandExecutor> commandExecutor,
                                                            const shared_ptr<IMenuProvider> menuProvider ) :
   _inputReader( inputReader ),
   _commandExecutor( commandExecutor ),
   _menuProvider( menuProvider )
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

   if ( _inputReader->WasButtonPressed( GameButton::Up ) )
   {
      _menuProvider->GetMenu( MenuType::Playing )->DecrementSelectedIndex();
   }
   else if ( _inputReader->WasButtonPressed( GameButton::Down ) )
   {
      _menuProvider->GetMenu( MenuType::Playing )->IncrementSelectedIndex();
   }
}
