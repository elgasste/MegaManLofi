#include "Stage.h"
#include "StageDefs.h"
#include "Arena.h"

using namespace std;
using namespace MegaManLofi;

Stage::Stage( const shared_ptr<StageDefs> stageDefs ) :
   _stageDefs( stageDefs )
{
   _activeArenaId = _stageDefs->StartArenaId;
}

void Stage::AddArena( std::shared_ptr<ReadOnlyArena> arena )
{
   _arenaMap[arena->GetArenaId()] = arena;
}

void Stage::Reset()
{
   auto activeArena = static_pointer_cast<Arena>( GetActiveArena() );
   activeArena->Clear();

   _activeArenaId = _stageDefs->StartArenaId;
}
