#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IGame
   {
   public:
      virtual void Tick() = 0;
   };
}
