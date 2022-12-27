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

shared_ptr<GameDefs> GameDefsGenerator::GenerateGameDefs( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto gameDefs = make_shared<GameDefs>();

   gameDefs->RenderDefs = ConsoleRenderDefsGenerator::GenerateConsoleRenderDefs( frameRateProvider );
   gameDefs->InputDefs = KeyboardInputDefsGenerator::GenerateKeyboardInputDefs();
   gameDefs->EntityDefs = EntityDefsGenerator::GenerateEntityDefs();
   gameDefs->PlayerDefs = PlayerDefsGenerator::GeneratePlayerDefs();
   gameDefs->ArenaDefs = ArenaDefsGenerator::GenerateArenaDefs();
   gameDefs->PlayerPhysicsDefs = PlayerPhysicsDefsGenerator::GeneratePlayerPhysicsDefs();

   return gameDefs;
}
