#include "PlayerDefsGenerator.h"
#include "PlayerDefs.h"
#include "UniqueNumberGenerator.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<PlayerDefs> PlayerDefsGenerator::GeneratePlayerDefs( const shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator )
{
   auto playerDefs = make_shared<PlayerDefs>();

   playerDefs->DefaultUniqueId = uniqueNumberGenerator->GetNext();
   playerDefs->DefaultEntityMetaId = 0;

   // one character is 38 x 78 units, and our player sprites are 4 x 3 characters,
   // so this hit box should match the player's sprite size
   playerDefs->DefaultHitBox = { 0, 0, 38 * 4, 78 * 3 };

   playerDefs->DefaultVelocityX = 0;
   playerDefs->DefaultVelocityY = 0;

   playerDefs->MaxGravityVelocity = 4'000;
   playerDefs->GravityAccelerationPerSecond = 10'000;
   playerDefs->FrictionDecelerationPerSecond = 10'000;

   playerDefs->DamageInvulnerabilitySeconds = 1.5f;

   playerDefs->MaxPushVelocity = 1'200;
   playerDefs->PushAccelerationPerSecond = 8'500;
   playerDefs->JumpAccelerationPerSecond = 4'000;
   playerDefs->MaxJumpExtensionSeconds = .25f;

   playerDefs->DefaultLives = 3;
   playerDefs->MaxHealth = 100;
   playerDefs->DefaultDirection = Direction::Right;
   playerDefs->DefaultMovementType = MovementType::Standing;

   return playerDefs;
}
