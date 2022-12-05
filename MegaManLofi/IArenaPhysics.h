#pragma once

#include <memory>

namespace MegaManLofi
{
   class IArena;
   class IPlayer;

   class __declspec( novtable ) IArenaPhysics
   {
   public:
      virtual void AssignTo( std::shared_ptr<IArena> arena,
                             std::shared_ptr<IPlayer> player ) = 0;
      virtual void Tick() = 0;
   };
}
