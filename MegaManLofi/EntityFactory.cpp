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
