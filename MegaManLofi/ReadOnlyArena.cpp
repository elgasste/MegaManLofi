#include "ReadOnlyArena.h"
#include "Entity.h"

using namespace std;
using namespace MegaManLofi;

ReadOnlyArena::ReadOnlyArena() :
   _arenaId( 0 ),
   _playerEntity( nullptr ),
   _width( 0 ),
   _height( 0 ),
   _tileWidth( 0 ),
   _tileHeight( 0 ),
   _horizontalTiles( 0 ),
   _verticalTiles( 0 ),
   _activeRegion( { 0, 0, 0, 0 } )
{
}

const shared_ptr<ReadOnlyEntity> ReadOnlyArena::GetEntity( int index ) const
{
   return _entities[index];
}

const shared_ptr<ReadOnlyEntity> ReadOnlyArena::GetPlayerEntity() const
{
   return _playerEntity;
}

bool ReadOnlyArena::HasEntity( int uniqueId ) const
{
   auto it = find_if( _entities.begin(), _entities.end(), [&uniqueId]( const shared_ptr<Entity>& entity )
   {
      return entity->GetUniqueId() == uniqueId;
   } );

   return it != _entities.end();
}
