#pragma once

#include <map>
#include <vector>
#include <functional>

#include "GameEvent.h"

namespace MegaManLofi
{
   class GameEventAggregator
   {
   public:
      virtual void RegisterEventHandler( GameEvent event, std::function<void()> handler );
      virtual void RaiseEvent( GameEvent event ) const;

   private:
      std::map<GameEvent, std::vector<std::function<void()>>> _eventHandlers;
   };
}
