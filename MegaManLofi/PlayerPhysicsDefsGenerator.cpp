#include "PlayerPhysicsDefsGenerator.h"
#include "PlayerPhysicsDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<PlayerPhysicsDefs> PlayerPhysicsDefsGenerator::GeneratePlayerPhysicsDefs()
{
   auto playerPhysicsDefs = make_shared<PlayerPhysicsDefs>();

   playerPhysicsDefs->MaxPushVelocity = 1'200'000;
   playerPhysicsDefs->MaxGravityVelocity = 4'000'000;

   playerPhysicsDefs->PushAccelerationPerSecond = 8'500'000;
   playerPhysicsDefs->FrictionDecelerationPerSecond = 10'000'000;
   playerPhysicsDefs->JumpAccelerationPerSecond = 2'000'000;
   playerPhysicsDefs->GravityAccelerationPerSecond = 10'000'000;

   playerPhysicsDefs->MaxJumpExtensionSeconds = 0.25;

   return playerPhysicsDefs;
}
