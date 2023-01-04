#include <algorithm>

#include "Arena.h"
#include "ArenaDefs.h"
#include "GameEventAggregator.h"
#include "Entity.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaDefs> arenaDefs,
              const shared_ptr<GameEventAggregator> eventAggregator ) :
   _arenaDefs( arenaDefs ),
   _eventAggregator( eventAggregator )
{
   _arenaId = arenaDefs->ArenaId;
   _tiles = arenaDefs->DefaultTiles;
   _width = arenaDefs->DefaultTileWidth * arenaDefs->DefaultHorizontalTiles;
   _height = arenaDefs->DefaultTileHeight * arenaDefs->DefaultVerticalTiles;
   _tileWidth = arenaDefs->DefaultTileWidth;
   _tileHeight = arenaDefs->DefaultTileHeight;
   _horizontalTiles = arenaDefs->DefaultHorizontalTiles;
   _verticalTiles = arenaDefs->DefaultVerticalTiles;

   Reset();
}

void Arena::Reset()
{
   _entities.clear();
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitiesCleared );

   if ( _playerEntity )
   {
      _playerEntity->SetArenaPosition( _arenaDefs->DefaultPlayerPosition );
      AddEntity( _playerEntity );
   }
}

void Arena::SetPlayerEntity( const shared_ptr<Entity> playerEntity )
{
   _playerEntity = playerEntity;
   AddEntity( playerEntity );
}

void Arena::AddEntity( const std::shared_ptr<Entity> entity )
{
   if ( find( _entities.begin(), _entities.end(), entity ) == _entities.end() )
   {
      _entities.push_back( entity );
      _eventAggregator->RaiseEvent( GameEvent::ArenaEntitySpawned );
   }
}

void Arena::RemoveEntity( const std::shared_ptr<Entity> entity )
{
   for ( auto it = _entities.begin(); it != _entities.end(); it++ )
   {
      if ( *it == entity )
      {
         _entities.erase( it );
         _eventAggregator->RaiseEvent( GameEvent::ArenaEntityDeSpawned );
         break;
      }
   }
}
