#pragma once

#include "GameButton.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IGameInputReader
   {
   public:
      virtual void ReadInput() = 0;
      virtual bool WasButtonPressed( GameButton button ) const = 0;
      virtual bool IsButtonDown( GameButton button ) const = 0;
      virtual bool WasAnyButtonPressed() const = 0;
   };
}
