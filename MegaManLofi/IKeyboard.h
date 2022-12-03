#pragma once

namespace MegaManLofi
{
   enum class KeyCode;

   class __declspec( novtable ) IKeyboard
   {
   public:
      virtual bool IsKeyDown( KeyCode keyCode ) const = 0;
   };
}
