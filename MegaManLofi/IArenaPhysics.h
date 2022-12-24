#pragma once

#include <memory>

namespace MegaManLofi
{
   class IArena;

   class __declspec( novtable ) IArenaPhysics
   {
   public:
      virtual void AssignTo( const std::shared_ptr<IArena> arena ) = 0;
      virtual void Tick() = 0;
   };
}
