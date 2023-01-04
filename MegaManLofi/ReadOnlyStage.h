#pragma once

#include <memory>
#include <map>

namespace MegaManLofi
{
   class ReadOnlyArena;

   class ReadOnlyStage
   {
   public:
      ReadOnlyStage();

      virtual const std::shared_ptr<ReadOnlyArena> GetArena( int arenaId ) const { return _arenaMap.at( arenaId ); }
      virtual const std::shared_ptr<ReadOnlyArena> GetActiveArena() const { return _arenaMap.at( _activeArenaId ); }

   private:
      std::map<int, std::shared_ptr<ReadOnlyArena>> _arenaMap;
      int _activeArenaId;
   };
}
