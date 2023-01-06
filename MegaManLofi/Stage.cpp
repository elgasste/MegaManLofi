#include "Stage.h"
#include "StageDefs.h"
#include "GameEventAggregator.h"
#include "Arena.h"

using namespace std;
using namespace MegaManLofi;

Stage::Stage( const shared_ptr<StageDefs> stageDefs,
              const shared_ptr<GameEventAggregator> eventAggregator ) :
   _stageDefs( stageDefs ),
   _eventAggregator( eventAggregator )
{
   Reload();
}

void Stage::AddArena( std::shared_ptr<Arena> arena )
{
   _arenaMap[arena->GetArenaId()] = arena;
}

void Stage::Reload()
{
   for ( auto [arenaId, arena] : _arenaMap )
   {
      arena->Reload();
   }

   _activeArenaId = _stageDefs->StartArenaId;
}

void Stage::Reset()
{
   for ( auto [arenaId, arena] : _arenaMap )
   {
      arena->Reset();
   }

   _activeArenaId = _stageDefs->StartArenaId;
}

void Stage::SetActiveArena( int arenaId )
{
   if ( arenaId != _activeArenaId )
   {
      _arenaMap[_activeArenaId]->Reset();
      _activeArenaId = arenaId;
      _eventAggregator->RaiseEvent( GameEvent::ActiveArenaChanged );
   }
}

const shared_ptr<ArenaPortal> Stage::GetArenaPortal( Direction direction, int fromArenaId ) const
{
   if ( _stageDefs->ArenaPortalMap.count( direction ) )
   {
      auto portals = _stageDefs->ArenaPortalMap.at( direction );

      for ( auto portal : portals )
      {
         if ( portal->FromArenaId == fromArenaId )
         {
            return portal;
         }
      }
   }

   return nullptr;
}
