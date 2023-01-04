#pragma once

#include <memory>
#include <map>

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

   protected:
      std::map<int, std::shared_ptr<Arena>> _arenaMap;
      int _activeArenaId;
   };
}
