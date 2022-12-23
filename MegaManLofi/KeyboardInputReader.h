#pragma once

#include <memory>
#include <map>
#include <vector>

#include "IGameInputReader.h"
#include "KeyCode.h"

namespace MegaManLofi
{
   struct ButtonState
   {
      bool ButtonWasPressed;
      bool ButtonIsDown;
   };

   class KeyboardInputDefs;
   class IKeyboard;

   class KeyboardInputReader : public IGameInputReader
   {
   public:
      KeyboardInputReader( const std::shared_ptr<KeyboardInputDefs> inputDefs,
                           const std::shared_ptr<IKeyboard> keyboard );

      void ReadInput() override;
      bool WasButtonPressed( GameButton button ) const override;
      bool IsButtonDown( GameButton button ) const override;
      bool WasAnyButtonPressed() const override;

   private:
      std::shared_ptr<IKeyboard> _keyboard;

      std::map<GameButton, ButtonState> _buttonStates;
      std::map<GameButton, std::vector<KeyCode>> _buttonKeyBindings;
   };
}
