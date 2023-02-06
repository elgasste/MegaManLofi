#include "EntityDefsGenerator.h"
#include "EntityDefs.h"
#include "GameDefGeneratorDefines.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<EntityDefs> EntityDefsGenerator::GenerateEntityDefs()
{
   auto entityDefs = make_shared<EntityDefs>();

   entityDefs->BulletEntityMetaId = METAID_PROJECTILE_BULLET;

   // meta ID 0 is reserved for the player
   entityDefs->EntityTypeMap[0] = EntityType::Player;

   /*************************** PROJECTILES ***************************/

   // bullet
   entityDefs->EntityTypeMap[METAID_PROJECTILE_BULLET] = EntityType::Projectile;
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_BULLET].HitBox = { 0, 0, 10, 10 }; // player is 152 x 234
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_BULLET].Velocity = 2'500;
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_BULLET].IsFriendly = true;
   entityDefs->CollisionPayloadMap[METAID_PROJECTILE_BULLET].Health = -10;

   /****************************** ITEMS ******************************/

   // small health drop
   entityDefs->EntityTypeMap[METAID_ITEM_SMALLHEALTHDROP] = EntityType::Item;
   entityDefs->ItemInfoMap[METAID_ITEM_SMALLHEALTHDROP].HitBox = { 0, 0, 10, 10 };
   entityDefs->ItemInfoMap[METAID_ITEM_SMALLHEALTHDROP].MaxGravityVelocity = 4'000;
   entityDefs->ItemInfoMap[METAID_ITEM_SMALLHEALTHDROP].GravityAccelerationPerSecond = 10'000;
   entityDefs->CollisionPayloadMap[METAID_ITEM_SMALLHEALTHDROP].Health = 10;

   // large health drop
   entityDefs->EntityTypeMap[METAID_ITEM_LARGEHEALTHDROP] = EntityType::Item;
   entityDefs->ItemInfoMap[METAID_ITEM_LARGEHEALTHDROP].HitBox = { 0, 0, 38, 78 }; // one full tile
   entityDefs->ItemInfoMap[METAID_ITEM_LARGEHEALTHDROP].MaxGravityVelocity = 4'000;
   entityDefs->ItemInfoMap[METAID_ITEM_LARGEHEALTHDROP].GravityAccelerationPerSecond = 10'000;
   entityDefs->CollisionPayloadMap[METAID_ITEM_LARGEHEALTHDROP].Health = 25;

   // extra life
   entityDefs->EntityTypeMap[METAID_ITEM_EXTRALIFE] = EntityType::Item;
   entityDefs->ItemInfoMap[METAID_ITEM_EXTRALIFE].HitBox = { 0, 0, 38, 78 }; // one full tile
   entityDefs->ItemInfoMap[METAID_ITEM_EXTRALIFE].MaxGravityVelocity = 4'000;
   entityDefs->ItemInfoMap[METAID_ITEM_EXTRALIFE].GravityAccelerationPerSecond = 10'000;
   entityDefs->CollisionPayloadMap[METAID_ITEM_EXTRALIFE].Lives = 1;

   /***************************** ENEMIES *****************************/

   // stationary turret
   entityDefs->EntityTypeMap[METAID_ENEMY_STATIONARYTURRET] = EntityType::Enemy;
   entityDefs->ItemInfoMap[METAID_ENEMY_STATIONARYTURRET].HitBox = { 0, 0, 38, 78 * 4 }; // 1 x 4 tiles
   entityDefs->ItemInfoMap[METAID_ENEMY_STATIONARYTURRET].MaxGravityVelocity = 4'000;
   entityDefs->ItemInfoMap[METAID_ENEMY_STATIONARYTURRET].GravityAccelerationPerSecond = 10'000;
   entityDefs->CollisionPayloadMap[METAID_ENEMY_STATIONARYTURRET].Health = -10;

   return entityDefs;
}
