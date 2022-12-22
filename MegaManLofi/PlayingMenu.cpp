#include "PlayingMenu.h"
#include "IGameCommandExecutor.h"
#include "GameCommand.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenu::PlayingMenu( const shared_ptr<IGameCommandExecutor> commandExecutor ) :
   _commandExecutor( commandExecutor ),
   _selectedIndex( 0 )
{
   _optionTitles.push_back( "Back to game" );
   _optionTitles.push_back( "Exit to title screen" );
   _optionTitles.push_back( "Quit game" );
}

void PlayingMenu::IncrementSelectedIndex()
{
   _selectedIndex++;

   if ( _selectedIndex >= (int)_optionTitles.size() )
   {
      _selectedIndex = 0;
   }
}

void PlayingMenu::DecrementSelectedIndex()
{
   _selectedIndex--;

   if ( _selectedIndex < 0 )
   {
      _selectedIndex = (int)_optionTitles.size() - 1;
   }
}

void PlayingMenu::ExecuteSelectedIndex()
{
   switch ( _selectedIndex )
   {
      case 0:
         _commandExecutor->ExecuteCommand( GameCommand::ClosePlayingMenu );
         break;
      case 1:
         _commandExecutor->ExecuteCommand( GameCommand::ExitToTitle );
         break;
      case 2:
         _commandExecutor->ExecuteCommand( GameCommand::Quit );
         break;
   }
}
