#include "ReadOnlyStage.h"
#include "Arena.h"

using namespace std;
using namespace MegaManLofi;

ReadOnlyStage::ReadOnlyStage() :
   _activeArenaId( 0 )
{
}

const shared_ptr<ReadOnlyArena> ReadOnlyStage::GetArena( int arenaId ) const
{
   return _arenaMap.at( arenaId );
}

const shared_ptr<ReadOnlyArena> ReadOnlyStage::GetActiveArena() const
{
   return _arenaMap.at( _activeArenaId );
}
