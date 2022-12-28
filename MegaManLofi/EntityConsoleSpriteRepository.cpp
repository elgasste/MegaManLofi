#include <vector>

#include "EntityConsoleSpriteRepository.h"
#include "IGameEventAggregator.h"
#include "IReadOnlyArena.h"
#include "ConsoleSpriteDefs.h"
#include "IReadOnlyEntity.h"

using namespace std;
using namespace MegaManLofi;

EntityConsoleSpriteRepository::EntityConsoleSpriteRepository( const shared_ptr<IGameEventAggregator> eventAggregator,
                                                              const shared_ptr<IReadOnlyArena> arena,
                                                              const shared_ptr<ConsoleSpriteDefs> spriteDefs ) :
   _arena( arena ),
   _spriteDefs( spriteDefs )
{
   eventAggregator->RegisterEventHandler( GameEvent::ArenaEntitySpawned, std::bind( &EntityConsoleSpriteRepository::HandleEntitySpawned, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::ArenaEntityDeSpawned, std::bind( &EntityConsoleSpriteRepository::HandleEntitySpawned, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::ArenaEntitiesCleared, std::bind( &EntityConsoleSpriteRepository::HandleEntitiesCleared, this ) );
}

const shared_ptr<IEntityConsoleSprite> EntityConsoleSpriteRepository::GetSprite( int uniqueId ) const
{
   // TODO: unit tests
   return _spriteMap.at( uniqueId );
}

void EntityConsoleSpriteRepository::HandleEntitySpawned()
{
   // TODO: unit tests
   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      auto entity = _arena->GetEntity( i );

      if ( _spriteMap.count( entity->GetUniqueId() ) == 0 )
      {
         auto sprite = _spriteDefs->EntitySpriteMap[entity->GetEntityMetaId()];

         // TODO: COPY this sprite to our map
      }
   }
}

void EntityConsoleSpriteRepository::HandleEntityDeSpawned()
{
   // TODO: unit tests
   vector<int> idsToRemove;

   for ( auto [uniqueId, entity] : _spriteMap )
   {
      // TODO:
      // 1) ask the arena if it has this entity
      // 2) if it doesn't, add the ID to the list of IDs to remove
   }

   for ( auto id : idsToRemove )
   {
      _spriteMap.erase( id );
   }
}

void EntityConsoleSpriteRepository::HandleEntitiesCleared()
{
   // TODO: unit tests
   _spriteMap.clear();
}
