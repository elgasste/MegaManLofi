#include "EntityDefsGenerator.h"
#include "EntityDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<EntityDefs> EntityDefsGenerator::GenerateEntityDefs()
{
   auto entityDefs = make_shared<EntityDefs>();

   entityDefs->BulletEntityMetaId = 1;
   entityDefs->BulletVelocity = 2'500;

   // the player is 152 x 234
   entityDefs->BulletHitBox = { 0, 0, 10, 10 };

   return entityDefs;
}
