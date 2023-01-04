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

      virtual void AddArena( std::shared_ptr<ReadOnlyArena> arena );
      virtual void Reset();
      virtual void SetActiveArena( int arenaId ) { _activeArenaId = arenaId; }

   private:
      const std::shared_ptr<StageDefs> _stageDefs;
   };
}
