#include "EntityFactory.h"
#include "EntityDefs.h"
#include "UniqueNumberGenerator.h"
#include "Entity.h"

using namespace std;
using namespace MegaManLofi;

EntityFactory::EntityFactory( const shared_ptr<EntityDefs> entityDefs,
                              const shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator ) :
   _entityDefs( entityDefs ),
   _uniqueNumberGenerator( uniqueNumberGenerator )
{
}

const shared_ptr<Entity> EntityFactory::CreateBullet( Coordinate<float> position, Direction direction ) const
{
   auto bullet = make_shared<Entity>();

   bullet->SetUniqueId( _uniqueNumberGenerator->GetNext() );
   bullet->SetEntityType( EntityType::Projectile );
   bullet->SetEntityMetaId( _entityDefs->BulletEntityMetaId );
   bullet->SetArenaPosition( position );
   bullet->SetDirection( direction );
   bullet->SetHitBox( _entityDefs->BulletHitBox );

   if ( direction == Direction::UpLeft || direction == Direction::Left || direction == Direction::DownLeft )
   {
      bullet->SetVelocityX( -_entityDefs->BulletVelocity );
   }
   else if ( direction == Direction::UpRight || direction == Direction::Right || direction == Direction::DownRight )
   {
      bullet->SetVelocityX( _entityDefs->BulletVelocity );
   }

   if ( direction == Direction::UpLeft || direction == Direction::Up || direction == Direction::UpRight )
   {
      bullet->SetVelocityY( -_entityDefs->BulletVelocity );
   }
   else if ( direction == Direction::DownLeft || direction == Direction::Down || direction == Direction::DownRight )
   {
      bullet->SetVelocityY( _entityDefs->BulletVelocity );
   }

   return bullet;
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
         entity->SetHitBox( _entityDefs->ItemInfoMap[entityMetaId].HitBox );
         break;
      case EntityType::Projectile:
         SetProjectileInfo( entity, direction );
         break;
   }

   return entity;
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
