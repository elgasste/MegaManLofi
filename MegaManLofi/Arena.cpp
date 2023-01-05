#include <algorithm>

#include "Arena.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"
#include "GameEventAggregator.h"
#include "Entity.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaDefs> arenaDefs,
              const shared_ptr<WorldDefs> worldDefs,
              const shared_ptr<GameEventAggregator> eventAggregator ) :
   _arenaDefs( arenaDefs ),
   _eventAggregator( eventAggregator )
{
   _arenaId = arenaDefs->ArenaId;
   _tiles = arenaDefs->Tiles;
   _width = worldDefs->TileWidth * arenaDefs->HorizontalTiles;
   _height = worldDefs->TileHeight * arenaDefs->VerticalTiles;
   _horizontalTiles = arenaDefs->HorizontalTiles;
   _verticalTiles = arenaDefs->VerticalTiles;

   Reset();
}

void Arena::Reset()
{
   _entities.clear();
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitiesCleared );

   if ( _playerEntity )
   {
      SetPlayerEntity( _playerEntity );
   }
}

void Arena::Clear()
{
   _entities.clear();
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitiesCleared );
   _playerEntity = nullptr;
}

void Arena::SetPlayerEntity( const shared_ptr<Entity> playerEntity )
{
   _playerEntity = playerEntity;
   _playerEntity->SetArenaPosition( _arenaDefs->PlayerStartPosition );
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
