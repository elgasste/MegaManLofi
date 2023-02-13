#include <filesystem>
#include <fstream>
#include <sstream>

#include "EntityDefsGenerator.h"
#include "EntityDefs.h"
#include "GameDefGeneratorDefines.h"
#include "MbcAsmCompiler.h"

using namespace std;
using namespace MegaManLofi;
using namespace MbcAsm;

shared_ptr<EntityDefs> EntityDefsGenerator::GenerateEntityDefs()
{
   auto entityDefs = make_shared<EntityDefs>();

   /***************************** PLAYER ******************************/
   
   entityDefs->EntityTypeMap[METAID_PLAYER] = EntityType::Player;

   /*************************** PROJECTILES ***************************/

   // bullet
   entityDefs->EntityTypeMap[METAID_PROJECTILE_GOODBULLET] = EntityType::Projectile;
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_GOODBULLET].HitBox = { 0, 0, 10, 10 }; // player is 152 x 234
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_GOODBULLET].Velocity = 2'500;
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_GOODBULLET].IsFriendly = true;
   entityDefs->CollisionPayloadMap[METAID_PROJECTILE_GOODBULLET].Health = -10;

   entityDefs->EntityTypeMap[METAID_PROJECTILE_BADBULLET] = EntityType::Projectile;
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_BADBULLET].HitBox = { 0, 0, 10, 10 }; // player is 152 x 234
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_BADBULLET].Velocity = 2'500;
   entityDefs->ProjectileInfoMap[METAID_PROJECTILE_BADBULLET].IsFriendly = false;
   entityDefs->CollisionPayloadMap[METAID_PROJECTILE_BADBULLET].Health = -10;

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
   entityDefs->ItemInfoMap[METAID_ITEM_EXTRALIFE].HitBox = { 0, 0, 38, 78 };
   entityDefs->ItemInfoMap[METAID_ITEM_EXTRALIFE].MaxGravityVelocity = 4'000;
   entityDefs->ItemInfoMap[METAID_ITEM_EXTRALIFE].GravityAccelerationPerSecond = 10'000;
   entityDefs->CollisionPayloadMap[METAID_ITEM_EXTRALIFE].Lives = 1;

   /***************************** ENEMIES *****************************/

   // stationary turret
   entityDefs->EntityTypeMap[METAID_ENEMY_STATIONARYTURRET] = EntityType::Enemy;
   entityDefs->EnemyInfoMap[METAID_ENEMY_STATIONARYTURRET].HitBox = { 0, 0, 38, 78 * 4 }; // 1 x 4 tiles
   entityDefs->EnemyInfoMap[METAID_ENEMY_STATIONARYTURRET].MaxGravityVelocity = 4'000;
   entityDefs->EnemyInfoMap[METAID_ENEMY_STATIONARYTURRET].GravityAccelerationPerSecond = 10'000;
   entityDefs->EnemyInfoMap[METAID_ENEMY_STATIONARYTURRET].MaxHealth = 80;
   entityDefs->EnemyInfoMap[METAID_ENEMY_STATIONARYTURRET].DamageInvulnerabilitySeconds = 0.1f;
   entityDefs->CollisionPayloadMap[METAID_ENEMY_STATIONARYTURRET].Health = -10;

   // spinning turret
   entityDefs->EntityTypeMap[METAID_ENEMY_SPINNINGTURRET] = EntityType::Enemy;
   entityDefs->EnemyInfoMap[METAID_ENEMY_SPINNINGTURRET].HitBox = { 0, 0, 38, 78 }; // one full tile
   entityDefs->EnemyInfoMap[METAID_ENEMY_SPINNINGTURRET].MaxHealth = 80;
   entityDefs->EnemyInfoMap[METAID_ENEMY_SPINNINGTURRET].DamageInvulnerabilitySeconds = 0.1f;
   entityDefs->CollisionPayloadMap[METAID_ENEMY_SPINNINGTURRET].Health = -10;

   // targeting turret
   entityDefs->EntityTypeMap[METAID_ENEMY_TARGETINGTURRET] = EntityType::Enemy;
   entityDefs->EnemyInfoMap[METAID_ENEMY_TARGETINGTURRET].HitBox = { 0, 0, 38, 78 }; // one full tile
   entityDefs->EnemyInfoMap[METAID_ENEMY_TARGETINGTURRET].MaxHealth = 80;
   entityDefs->EnemyInfoMap[METAID_ENEMY_TARGETINGTURRET].DamageInvulnerabilitySeconds = 0.1f;
   entityDefs->CollisionPayloadMap[METAID_ENEMY_TARGETINGTURRET].Health = -10;

   // bad dude
   entityDefs->EntityTypeMap[METAID_ENEMY_BADDUDE] = EntityType::Enemy;
   entityDefs->EnemyInfoMap[METAID_ENEMY_BADDUDE].HitBox = { 0, 0, 38 * 4, 78 * 3 }; // 4 x 3 tiles
   entityDefs->EnemyInfoMap[METAID_ENEMY_BADDUDE].MaxHealth = 100;
   entityDefs->EnemyInfoMap[METAID_ENEMY_BADDUDE].DamageInvulnerabilitySeconds = 1.0f;
   entityDefs->CollisionPayloadMap[METAID_ENEMY_BADDUDE].Health = -8;

   /*********************** ENTITY PROJECTILES ************************/

   entityDefs->EntityProjectileMap[METAID_PLAYER] = METAID_PROJECTILE_GOODBULLET;
   entityDefs->EntityProjectileMap[METAID_ENEMY_STATIONARYTURRET] = METAID_PROJECTILE_BADBULLET;
   entityDefs->EntityProjectileMap[METAID_ENEMY_SPINNINGTURRET] = METAID_PROJECTILE_BADBULLET;
   entityDefs->EntityProjectileMap[METAID_ENEMY_TARGETINGTURRET] = METAID_PROJECTILE_BADBULLET;
   entityDefs->EntityProjectileMap[METAID_ENEMY_BADDUDE] = METAID_PROJECTILE_BADBULLET;

   GenerateEntityBehaviors( entityDefs );

   return entityDefs;
}

void EntityDefsGenerator::GenerateEntityBehaviors( shared_ptr<EntityDefs> entityDefs )
{
   MbcAsmCompiler compiler;

   filesystem::path thisFilePath( __FILE__ );
   auto asmPath = thisFilePath.parent_path() / "BehaviorASM";

   for ( auto& filePath : filesystem::directory_iterator( asmPath ) )
   {
      if ( filePath.path().extension() != ".mbcasm" )
      {
         continue;
      }

      auto fileName = filePath.path().stem();
      auto metaId = stoi( fileName ); // this will throw if it's not an int, I think that's fine?

      ifstream asmStream( filePath.path() );
      stringstream asmBuffer;
      asmBuffer << asmStream.rdbuf();

      auto mbc = compiler.Compile( asmBuffer.str() );
      entityDefs->EntityBehaviorMap[metaId] = mbc;
   }
}
