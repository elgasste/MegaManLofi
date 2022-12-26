#include "EntityDefsGenerator.h"
#include "EntityDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<EntityDefs> EntityDefsGenerator::GenerateEntityDefs()
{
   auto entityDefs = make_shared<EntityDefs>();

   entityDefs->BulletVelocity = 2'000'000;

   // the player is 152,000 x 234,000
   entityDefs->BulletHitBox = { 0, 0, 10'000, 10'000 };

   return entityDefs;
}
