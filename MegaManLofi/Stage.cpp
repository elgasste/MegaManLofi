#include "Stage.h"
#include "StageDefs.h"
#include "ReadOnlyArena.h"

using namespace std;
using namespace MegaManLofi;

Stage::Stage( const shared_ptr<StageDefs> stageDefs ) :
   _stageDefs( stageDefs )
{
   Reset();
}

void Stage::AddArena( std::shared_ptr<ReadOnlyArena> arena )
{
   _arenaMap[arena->GetArenaId()] = arena;
}

void Stage::Reset()
{
   _activeArenaId = _stageDefs->StartArenaId;
}
