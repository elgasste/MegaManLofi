#pragma once

#include <memory>

namespace MegaManLofi
{
   class KeyboardInputDefs;

   class KeyboardInputDefsGenerator
   {
   public:
      static std::shared_ptr<KeyboardInputDefs> GenerateKeyboardInputDefs();
   };
}
