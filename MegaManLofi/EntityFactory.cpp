#include "EntityFactory.h"
#include "EntityDefs.h"
#include "UniqueNumberGenerator.h"
#include "Entity.h"
#include "EntityBehavior.h"

using namespace std;
using namespace MegaManLofi;

EntityFactory::EntityFactory( const shared_ptr<EntityDefs> entityDefs,
                              const shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator ) :
   _entityDefs( entityDefs ),
   _uniqueNumberGenerator( uniqueNumberGenerator )
{
}

const shared_ptr<Entity> EntityFactory::CreateEntity( int entityMetaId, Direction direction ) const
{
   auto type = _entityDefs->EntityTypeMap[entityMetaId];
   auto entity = make_shared<Entity>();

   entity->SetUniqueId( _uniqueNumberGenerator->GetNext() );
   entity->SetEntityType( type );
   entity->SetEntityMetaId( entityMetaId );
   entity->SetDirection( direction );

   switch ( type )
   {
      case EntityType::Item:
         SetItemInfo( entity, entityMetaId );
         break;
      case EntityType::Projectile:
         SetProjectileInfo( entity, direction );
         break;
      case EntityType::Enemy:
         SetEnemyInfo( entity, entityMetaId );
         break;
   }

   return entity;
}

void EntityFactory::SetItemInfo( const std::shared_ptr<Entity> item, int metaId ) const
{
   item->SetHitBox( _entityDefs->ItemInfoMap[metaId].HitBox );
   item->SetMaxGravityVelocity( _entityDefs->ItemInfoMap[metaId].MaxGravityVelocity );
   item->SetGravityAccelerationPerSecond( _entityDefs->ItemInfoMap[metaId].GravityAccelerationPerSecond );
}

void EntityFactory::SetProjectileInfo( const std::shared_ptr<Entity> projectile, Direction direction ) const
{
   auto metaId = projectile->GetEntityMetaId();
   auto velocity = _entityDefs->ProjectileInfoMap[metaId].Velocity;

   projectile->SetHitBox( _entityDefs->ProjectileInfoMap[metaId].HitBox );

   if ( direction == Direction::UpLeft || direction == Direction::Left || direction == Direction::DownLeft )
   {
      projectile->SetVelocityX( -velocity );
   }
   else if ( direction == Direction::UpRight || direction == Direction::Right || direction == Direction::DownRight )
   {
      projectile->SetVelocityX( velocity );
   }

   if ( direction == Direction::UpLeft || direction == Direction::Up || direction == Direction::UpRight )
   {
      projectile->SetVelocityY( -velocity );
   }
   else if ( direction == Direction::DownLeft || direction == Direction::Down || direction == Direction::DownRight )
   {
      projectile->SetVelocityY( velocity );
   }
}

void EntityFactory::SetEnemyInfo( const std::shared_ptr<Entity> enemy, int metaId ) const
{
   enemy->SetHitBox( _entityDefs->EnemyInfoMap[metaId].HitBox );
   enemy->SetMaxGravityVelocity( _entityDefs->EnemyInfoMap[metaId].MaxGravityVelocity );
   enemy->SetGravityAccelerationPerSecond( _entityDefs->EnemyInfoMap[metaId].GravityAccelerationPerSecond );

   auto behaviorIt = _entityDefs->EntityBehaviorMap.find( metaId );

   if ( behaviorIt != _entityDefs->EntityBehaviorMap.end() )
   {
      auto behavior = shared_ptr<EntityBehavior>( new EntityBehavior( *behaviorIt->second ) );
      enemy->SetBehavior( shared_ptr<EntityBehavior>( behavior ) );
      behavior->AssignTo( enemy );
   }
}
