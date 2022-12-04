#pragma once

#include <memory>

namespace MegaManLofi
{
   class IArena;
   class IPlayer;

   class __declspec( novtable ) IArenaPhysics
   {
   public:
      virtual void MovePlayer( const std::shared_ptr<IArena> arena,
                               const std::shared_ptr<IPlayer> player ) const = 0;
   };
}
