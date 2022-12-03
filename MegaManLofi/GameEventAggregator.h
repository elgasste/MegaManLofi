#pragma once

#include <map>
#include <vector>

#include "IGameEventAggregator.h"

namespace MegaManLofi
{
   enum class GameEvent;

   class GameEventAggregator : public IGameEventAggregator
   {
   public:
      void RegisterEventHandler( GameEvent event, std::function<void()> handler ) override;
      void RaiseEvent( GameEvent event ) const override;

   private:
      std::map<GameEvent, std::vector<std::function<void()>>> _eventHandlers;
   };
}
