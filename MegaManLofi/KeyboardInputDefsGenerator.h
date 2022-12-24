#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameInputDefs;

   class KeyboardInputDefsGenerator
   {
   public:
      static std::shared_ptr<IGameInputDefs> GenerateKeyboardInputDefs();
   };
}
