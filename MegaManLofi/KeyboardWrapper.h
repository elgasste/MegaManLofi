#pragma once

#include "IKeyboard.h"

namespace MegaManLofi
{
   class KeyboardWrapper : public IKeyboard
   {
   public:
      bool IsKeyDown( KeyCode keyCode ) const override;
   };
}
