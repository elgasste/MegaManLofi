#include "EntityFactory.h"
#include "EntityDefs.h"
#include "UniqueNumberGenerator.h"
#include "Entity.h"
#include "EntityBehavior.h"

using namespace std;
using namespace MegaManLofi;

EntityFactory::EntityFactory( const shared_ptr<EntityDefs> entityDefs,
                              const shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator,
                              const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _entityDefs( entityDefs ),
   _uniqueNumberGenerator( uniqueNumberGenerator ),
   _frameRateProvider( frameRateProvider )
{
}

void EntityFactory::Initialize( const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                const shared_ptr<IGameCommandExecutor> commandExecutor )
{
   _playerInfoProvider = playerInfoProvider;
   _commandExecutor = commandExecutor;
}

const shared_ptr<Entity> EntityFactory::CreateEntity( int entityMetaId,
                                                      const Coordinate<float>& position,
                                                      Direction direction ) const
{
   auto type = _entityDefs->EntityTypeMap[entityMetaId];

   switch ( type )
   {
      case EntityType::Item:
         return CreateItem( entityMetaId, position, direction );
      case EntityType::Projectile:
         return CreateProjectile( entityMetaId, position, direction );
      case EntityType::Enemy:
         return CreateEnemy( entityMetaId, position, direction );
      default:
         return nullptr;
   }
}

const shared_ptr<Entity> EntityFactory::CreateItem( int itemMetaId,
                                                    const Coordinate<float>& position,
                                                    Direction direction ) const
{
   auto item = shared_ptr<Entity>( new Entity( _frameRateProvider ) );
   SetEntityBaseInfo( item, EntityType::Item, itemMetaId, position, direction );
   const auto& itemInfo = _entityDefs->ItemInfoMap.at( itemMetaId );

   item->SetHitBox( itemInfo.HitBox );
   item->SetMaxGravityVelocity( itemInfo.MaxGravityVelocity );
   item->SetGravityAccelerationPerSecond( itemInfo.GravityAccelerationPerSecond );
   item->SetMaxHealth( 1 );
   item->SetHealth( 1 );

   return item;
}

const shared_ptr<Entity> EntityFactory::CreateProjectile( int projectileMetaId,
                                                          const Coordinate<float>& position,
                                                          Direction direction ) const
{
   auto projectile = shared_ptr<Entity>( new Entity( _frameRateProvider ) );
   SetEntityBaseInfo( projectile, EntityType::Projectile, projectileMetaId, position, direction );
   SetProjectileBaseInfo( projectile, projectileMetaId );
   auto velocity = _entityDefs->ProjectileInfoMap.at( projectileMetaId ).Velocity;

   if ( direction == Direction::Left )
   {
      projectile->SetVelocityX( -velocity );
   }
   if ( direction == Direction::UpLeft || direction == Direction::DownLeft )
   {
      projectile->SetVelocityX( -( velocity / 2.0f ) );
   }
   else if ( direction == Direction::Right )
   {
      projectile->SetVelocityX( velocity );
   }
   else if ( direction == Direction::UpRight || direction == Direction::DownRight )
   {
      projectile->SetVelocityX( velocity / 2.0f );
   }

   if ( direction == Direction::Up )
   {
      projectile->SetVelocityY( -velocity );
   }
   else if ( direction == Direction::UpLeft || direction == Direction::UpRight )
   {
      projectile->SetVelocityY( -( velocity / 2.0f ) );
   }
   else if ( direction == Direction::Down )
   {
      projectile->SetVelocityY( velocity );
   }
   else if ( direction == Direction::DownLeft || direction == Direction::DownRight )
   {
      projectile->SetVelocityY( velocity / 2.0f );
   }

   return projectile;
}

const shared_ptr<Entity> EntityFactory::CreateProjectile( int projectileMetaId,
                                                          const Coordinate<float>& position,
                                                          const Coordinate<float>& target ) const
{
   auto projectile = shared_ptr<Entity>( new Entity( _frameRateProvider ) );
   SetEntityBaseInfo( projectile, EntityType::Projectile, projectileMetaId, position, (Direction)0 );
   SetProjectileBaseInfo( projectile, projectileMetaId );
   auto velocity = _entityDefs->ProjectileInfoMap.at( projectileMetaId ).Velocity;
   
   // TODO: the math

   return projectile;
}

// TODO: figure out how to test this function
const shared_ptr<Entity> EntityFactory::CreateEnemy( int enemyMetaId,
                                                     const Coordinate<float>& position,
                                                     Direction direction ) const
{
   auto enemy = shared_ptr<Entity>( new Entity( _frameRateProvider ) );
   SetEntityBaseInfo( enemy, EntityType::Enemy, enemyMetaId, position, direction );

   const auto& enemyInfo = _entityDefs->EnemyInfoMap.at( enemyMetaId );

   enemy->SetHitBox( enemyInfo.HitBox );
   enemy->SetMaxGravityVelocity( enemyInfo.MaxGravityVelocity );
   enemy->SetGravityAccelerationPerSecond( enemyInfo.GravityAccelerationPerSecond );
   enemy->SetMaxHealth( enemyInfo.MaxHealth );
   enemy->SetHealth( enemy->GetMaxHealth() );
   enemy->SetDamageInvulnerabilitySeconds( enemyInfo.DamageInvulnerabilitySeconds );

   auto behaviorIt = _entityDefs->EntityBehaviorMap.find( enemyMetaId );

   if ( behaviorIt != _entityDefs->EntityBehaviorMap.end() )
   {
      auto behavior = shared_ptr<EntityBehavior>( new EntityBehavior( _frameRateProvider, _playerInfoProvider, _commandExecutor ) );
      enemy->SetBehavior( shared_ptr<EntityBehavior>( behavior ) );
      behavior->AssignTo( enemy );
      behavior->SetInstructions( _entityDefs->EntityBehaviorMap.at( enemyMetaId ) );
   }

   return enemy;
}

void EntityFactory::SetEntityBaseInfo( shared_ptr<Entity> entity,
                                       EntityType type,
                                       int entityMetaId,
                                       const Coordinate<float>& position,
                                       Direction direction ) const
{
   entity->SetUniqueId( _uniqueNumberGenerator->GetNext() );
   entity->SetEntityType( type );
   entity->SetEntityMetaId( entityMetaId );
   entity->SetArenaPosition( position );
   entity->SetDirection( direction );
}

void EntityFactory::SetProjectileBaseInfo( shared_ptr<Entity> projectile, int projectileMetaId ) const
{
   projectile->SetHitBox( _entityDefs->ProjectileInfoMap.at( projectileMetaId ).HitBox );
   projectile->SetMaxHealth( 1 );
   projectile->SetHealth( 1 );
}
