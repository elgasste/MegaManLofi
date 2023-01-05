#pragma once

#include <memory>
#include <map>
#include <vector>
#include <optional>

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
      virtual std::optional<std::reference_wrapper<const ArenaPortal>> GetArenaPortal( Direction direction, int fromArenaId ) const;

   protected:
      std::map<int, std::shared_ptr<Arena>> _arenaMap;
      std::map<Direction, std::vector<ArenaPortal>> _arenaPortalMap;
      int _activeArenaId;
   };
}
