#include "GameEventAggregator.h"

using namespace std;
using namespace MegaManLofi;

void GameEventAggregator::RegisterEventHandler( GameEvent event, function<void()> handler )
{
   _eventHandlers[event].push_back( handler );
}

void GameEventAggregator::RaiseEvent( GameEvent event ) const
{
   if ( _eventHandlers.count( event ) )
   {
      auto handlers = _eventHandlers.at( event );

      for( auto handler : handlers )
      {
         handler();
      }
   }
}
