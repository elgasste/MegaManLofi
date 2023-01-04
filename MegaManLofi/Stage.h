#pragma once

#include "ReadOnlyStage.h"

namespace MegaManLofi
{
   class StageDefs;

   class Stage : public ReadOnlyStage
   {
   public:
      Stage() { }
      Stage( const std::shared_ptr<StageDefs> stageDefs );

      virtual const std::shared_ptr<Arena> GetMutableActiveArena() const { return _arenaMap.at( _activeArenaId ); }
      virtual void AddArena( std::shared_ptr<Arena> arena );
      virtual void Reset();
      virtual void SetActiveArena( int arenaId ) { _activeArenaId = arenaId; }

   private:
      const std::shared_ptr<StageDefs> _stageDefs;
   };
}
