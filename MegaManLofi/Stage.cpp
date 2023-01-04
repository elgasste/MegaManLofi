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
   _activeArenaId = _stageDefs->StartArenaId;
}

void Stage::AddArena( std::shared_ptr<Arena> arena )
{
   _arenaMap[arena->GetArenaId()] = arena;
}

void Stage::Reset()
{
   _arenaMap[_activeArenaId]->Clear();
   _activeArenaId = _stageDefs->StartArenaId;
}

void Stage::SetActiveArena( int arenaId )
{
   if ( arenaId != _activeArenaId )
   {
      _arenaMap[_activeArenaId]->Clear();
      _activeArenaId = arenaId;
      _eventAggregator->RaiseEvent( GameEvent::ActiveArenaChanged );
   }
}
