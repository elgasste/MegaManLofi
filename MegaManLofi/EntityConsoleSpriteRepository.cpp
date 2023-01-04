#include <vector>

#include "EntityConsoleSpriteRepository.h"
#include "GameEventAggregator.h"
#include "ReadOnlyStage.h"
#include "ReadOnlyArena.h"
#include "EntityConsoleSpriteCopier.h"
#include "ConsoleSpriteDefs.h"
#include "ReadOnlyEntity.h"
#include "EntityConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

EntityConsoleSpriteRepository::EntityConsoleSpriteRepository( const shared_ptr<GameEventAggregator> eventAggregator,
                                                              const shared_ptr<ReadOnlyStage> stage,
                                                              const shared_ptr<EntityConsoleSpriteCopier> spriteCopier,
                                                              const shared_ptr<ConsoleSpriteDefs> spriteDefs ) :
   _stage( stage ),
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
   auto arena = _stage->GetActiveArena();

   for ( int i = 0; i < arena->GetEntityCount(); i++ )
   {
      auto entity = arena->GetEntity( i );
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
      if ( !_stage->GetActiveArena()->HasEntity( uniqueId ) )
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
