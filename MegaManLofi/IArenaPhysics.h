#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IArenaPhysics
   {
   public:
      virtual void Tick() = 0;
   };
}
