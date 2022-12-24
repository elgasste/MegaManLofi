#include "KeyboardInputReader.h"
#include "KeyboardInputDefs.h"
#include "IKeyboard.h"

using namespace std;
using namespace MegaManLofi;

KeyboardInputReader::KeyboardInputReader( const shared_ptr<KeyboardInputDefs> inputDefs,
                                          const shared_ptr<IKeyboard> keyboard ) :
   _keyboard( keyboard )
{
   for ( int i = 0; i < (int)GameButton::GameButtonCount; i++ )
   {
      _buttonStates[(GameButton)i] = { false, false };
   }

   // since we allow multiple keys to bind to a single button, creating this inverted
   // map of button-to-keys makes the input reading logic much easier
   for ( int i = 0; i < (int)GameButton::GameButtonCount; i++ )
   {
      auto button = (GameButton)i;

      for ( auto const& [keyCode, mappedButton] : inputDefs->KeyMap )
      {
         if ( mappedButton == button )
         {
            _buttonKeyBindings[button].push_back( keyCode );
         }
      }
   }
}

void KeyboardInputReader::ReadInput()
{
   for ( auto const& [button, keyCodes] : _buttonKeyBindings )
   {
      bool buttonIsDown = false;

      for ( auto keyCode : keyCodes )
      {
         if ( _keyboard->IsKeyDown( keyCode ) )
         {
            buttonIsDown = true;
            break;
         }
      }

      if ( buttonIsDown )
      {
         _buttonStates.at( button ).ButtonWasPressed = !_buttonStates.at( button ).ButtonIsDown;
         _buttonStates.at( button ).ButtonIsDown = true;
      }
      else
      {
         _buttonStates.at( button ).ButtonWasPressed = false;
         _buttonStates.at( button ).ButtonIsDown = false;
      }
   }
}

bool KeyboardInputReader::WasButtonPressed( GameButton button ) const
{
   return _buttonStates.at( button ).ButtonWasPressed;
}

bool KeyboardInputReader::IsButtonDown( GameButton button ) const
{
   return _buttonStates.at( button ).ButtonIsDown;
}

bool KeyboardInputReader::WasAnyButtonPressed() const
{
   for ( int i = 0; i < (int)GameButton::GameButtonCount; i++ )
   {
      auto button = (GameButton)i;

      if ( button != GameButton::Diagnostics && WasButtonPressed( button ) )
      {
         return true;
      }
   }

   return false;
}
