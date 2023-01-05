#pragma once

#include <memory>
#include <map>
#include <vector>

#include "ArenaPortal.h"
#include "Direction.h"

namespace MegaManLofi
{
   class Arena;
   class ReadOnlyArena;

   class ReadOnlyStage
   {
   public:
      ReadOnlyStage();

      virtual const std::shared_ptr<ReadOnlyArena> GetArena( int arenaId ) const;
      virtual const std::shared_ptr<ReadOnlyArena> GetActiveArena() const;
      virtual const std::shared_ptr<ArenaPortal> GetArenaPortal( Direction direction, int fromArenaId ) const = 0;

   protected:
      std::map<int, std::shared_ptr<Arena>> _arenaMap;
      int _activeArenaId;
   };
}
