#pragma once

#include "ReadOnlyStage.h"

namespace MegaManLofi
{
   class StageDefs;
   class GameEventAggregator;

   class Stage : public ReadOnlyStage
   {
   public:
      Stage() { }
      Stage( const std::shared_ptr<StageDefs> stageDefs,
             const std::shared_ptr<GameEventAggregator> eventAggregator );

      virtual const std::shared_ptr<Arena> GetMutableActiveArena() const { return _arenaMap.at( _activeArenaId ); }
      virtual void AddArena( std::shared_ptr<Arena> arena );
      virtual void Reload();
      virtual void Reset();
      virtual void SetActiveArena( int arenaId );

      virtual const std::shared_ptr<ArenaPortal> GetArenaPortal( Direction direction, int fromArenaId ) const override;

   private:
      const std::shared_ptr<StageDefs> _stageDefs;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
   };
}
