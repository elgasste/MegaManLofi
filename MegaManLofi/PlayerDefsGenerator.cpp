#include "PlayerDefsGenerator.h"
#include "PlayerDefs.h"
#include "IUniqueNumberGenerator.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<PlayerDefs> PlayerDefsGenerator::GeneratePlayerDefs( const shared_ptr<IUniqueNumberGenerator> uniqueNumberGenerator )
{
   auto playerDefs = make_shared<PlayerDefs>();

   playerDefs->DefaultUniqueId = uniqueNumberGenerator->GetNext();
   playerDefs->DefaultEntityMetaId = 0;

   // one character is 38 x 78 units, and our player sprites are 4 x 3 characters,
   // so this hit box should match the player's sprite size
   playerDefs->DefaultHitBox = { 0, 0, 38 * 4, 78 * 3 };

   playerDefs->DefaultVelocityX = 0;
   playerDefs->DefaultVelocityY = 0;

   playerDefs->DefaultLives = 3;
   playerDefs->DefaultDirection = Direction::Right;
   playerDefs->DefaultMovementType = MovementType::Standing;

   return playerDefs;
}
