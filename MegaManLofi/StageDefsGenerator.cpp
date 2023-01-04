#include "StageDefsGenerator.h"
#include "ArenaDefsGenerator.h"
#include "StageDefs.h"
#include "ArenaDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<StageDefs> StageDefsGenerator::GenerateStageDefs( shared_ptr<WorldDefs> worldDefs )
{
   auto stageDefs = make_shared<StageDefs>();

   stageDefs->StartArenaId = 0;

   stageDefs->ArenaMap[0] = ArenaDefsGenerator::GenerateArenaDefs( worldDefs );

   return stageDefs;
}
