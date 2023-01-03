#include "GameDefsGenerator.h"
#include "GameDefs.h"
#include "ConsoleRenderDefsGenerator.h"
#include "KeyboardInputDefsGenerator.h"
#include "EntityDefsGenerator.h"
#include "PlayerDefsGenerator.h"
#include "ArenaDefsGenerator.h"
#include "PlayerPhysicsDefsGenerator.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<GameDefs> GameDefsGenerator::GenerateGameDefs( const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                          const shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator )
{
   auto gameDefs = make_shared<GameDefs>();

   // the player's maximum Y-velocity is 4,000 and a tile is 78 units tall, so this
   // should prevent the player from clipping through any surfaces when the frame rate drops
   gameDefs->MinimumFrameRate = 52;

   gameDefs->RenderDefs = ConsoleRenderDefsGenerator::GenerateConsoleRenderDefs( frameRateProvider );
   gameDefs->InputDefs = KeyboardInputDefsGenerator::GenerateKeyboardInputDefs();
   gameDefs->EntityDefs = EntityDefsGenerator::GenerateEntityDefs();
   gameDefs->PlayerDefs = PlayerDefsGenerator::GeneratePlayerDefs( uniqueNumberGenerator );
   gameDefs->ArenaDefs = ArenaDefsGenerator::GenerateArenaDefs();
   gameDefs->PlayerPhysicsDefs = PlayerPhysicsDefsGenerator::GeneratePlayerPhysicsDefs();

   return gameDefs;
}
