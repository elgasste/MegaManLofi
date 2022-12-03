#pragma once

#include <functional>

namespace MegaManLofi
{
   enum class GameEvent;

   class __declspec( novtable ) IGameEventAggregator
   {
   public:
      virtual void RegisterEventHandler( GameEvent event, std::function<void()> handler ) = 0;
      virtual void RaiseEvent( GameEvent event ) const = 0;
   };
}
