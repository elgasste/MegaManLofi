#include <vector>

#include "EntityConsoleSpriteRepository.h"
#include "IGameEventAggregator.h"
#include "ReadOnlyArena.h"
#include "EntityConsoleSpriteCopier.h"
#include "ConsoleSpriteDefs.h"
#include "ReadOnlyEntity.h"
#include "EntityConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

EntityConsoleSpriteRepository::EntityConsoleSpriteRepository( const shared_ptr<IGameEventAggregator> eventAggregator,
                                                              const shared_ptr<ReadOnlyArena> arena,
                                                              const shared_ptr<EntityConsoleSpriteCopier> spriteCopier,
                                                              const shared_ptr<ConsoleSpriteDefs> spriteDefs ) :
   _arena( arena ),
   _spriteCopier( spriteCopier ),
   _spriteDefs( spriteDefs )
{
   eventAggregator->RegisterEventHandler( GameEvent::ArenaEntitySpawned, std::bind( &EntityConsoleSpriteRepository::HandleEntitySpawned, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::ArenaEntityDeSpawned, std::bind( &EntityConsoleSpriteRepository::HandleEntityDeSpawned, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::ArenaEntitiesCleared, std::bind( &EntityConsoleSpriteRepository::HandleEntitiesCleared, this ) );
}

const shared_ptr<EntityConsoleSprite> EntityConsoleSpriteRepository::GetSprite( int uniqueId ) const
{
   return _spriteMap.at( uniqueId );
}

void EntityConsoleSpriteRepository::HandleEntitySpawned()
{
   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      auto entity = _arena->GetEntity( i );
      auto uniqueId = entity->GetUniqueId();

      if ( _spriteMap.count( uniqueId ) == 0 )
      {
         auto sprite = _spriteDefs->EntitySpriteMap[entity->GetEntityMetaId()];
         auto spriteCopy = _spriteCopier->MakeCopy( sprite );
         spriteCopy->AssignTo( entity );

         _spriteMap[uniqueId] = spriteCopy;
      }
   }
}

void EntityConsoleSpriteRepository::HandleEntityDeSpawned()
{
   vector<int> idsToRemove;

   for ( auto [uniqueId, entity] : _spriteMap )
   {
      if ( !_arena->HasEntity( uniqueId ) )
      {
         idsToRemove.push_back( uniqueId );
      }
   }

   for ( auto id : idsToRemove )
   {
      _spriteMap.erase( id );
   }
}

void EntityConsoleSpriteRepository::HandleEntitiesCleared()
{
   _spriteMap.clear();
}
