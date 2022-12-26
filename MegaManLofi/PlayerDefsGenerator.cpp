#include "PlayerDefsGenerator.h"
#include "PlayerDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<PlayerDefs> PlayerDefsGenerator::GeneratePlayerDefs()
{
   auto playerDefs = make_shared<PlayerDefs>();

   // one character is 38,000 x 78,000 units, and our player sprites are 4 x 3 characters,
   // so this hit box should match the player's sprite size
   playerDefs->DefaultHitBox = { 0, 0, 38'000 * 4, 78'000 * 3 };

   playerDefs->DefaultVelocityX = 0;
   playerDefs->DefaultVelocityY = 0;

   playerDefs->DefaultLives = 3;
   playerDefs->DefaultDirection = Direction::Right;

   return playerDefs;
}