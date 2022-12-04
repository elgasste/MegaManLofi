#pragma once

#include "IArenaPhysics.h"

namespace MegaManLofi
{
   class ArenaPhysics : public IArenaPhysics
   {
   public:
      void MovePlayer( const std::shared_ptr<IArena> arena,
                       const std::shared_ptr<IPlayer> player ) const override;
   };
}
