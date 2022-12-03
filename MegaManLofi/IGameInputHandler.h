#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IGameInputHandler
   {
   public:
      virtual void HandleInput() = 0;
   };
}
