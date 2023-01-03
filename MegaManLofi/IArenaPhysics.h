#pragma once

#include <memory>

namespace MegaManLofi
{
   class Arena;

   class __declspec( novtable ) IArenaPhysics
   {
   public:
      virtual void AssignTo( const std::shared_ptr<Arena> arena ) = 0;
      virtual void Tick() = 0;
   };
}
