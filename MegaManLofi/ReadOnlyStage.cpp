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

optional<reference_wrapper<const ArenaPortal>> ReadOnlyStage::GetArenaPortal( Direction direction, int fromArenaId ) const
{
   if ( _arenaPortalMap.count( direction ) )
   {
      auto portals = _arenaPortalMap.at( direction );

      for ( auto portal : portals )
      {
         if ( portal.FromArenaId == fromArenaId )
         {
            return portal;
         }
      }
   }

   return { };
}
