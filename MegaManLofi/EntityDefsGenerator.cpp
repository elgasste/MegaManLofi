#include "EntityDefsGenerator.h"
#include "EntityDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<EntityDefs> EntityDefsGenerator::GenerateEntityDefs()
{
   // TODO: remove this stuff when we start using the maps below
   auto entityDefs = make_shared<EntityDefs>();

   entityDefs->BulletEntityMetaId = 1;
   entityDefs->BulletVelocity = 2'500;

   // the player is 152 x 234
   entityDefs->BulletHitBox = { 0, 0, 10, 10 };

   entityDefs->EntityTypeMap[0] = EntityType::Player;
   entityDefs->EntityTypeMap[1] = EntityType::Projectile; // bullet

   // bullet
   entityDefs->ProjectileInfoMap[1].HitBox = { 0, 0, 10, 10 }; // player is 152 x 234
   entityDefs->ProjectileInfoMap[1].Velocity = 2'500;

   return entityDefs;
}
