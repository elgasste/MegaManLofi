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

   stageDefs->ArenaMap = ArenaDefsGenerator::GenerateArenaDefsMap( worldDefs );

   stageDefs->ArenaPortalMap[Direction::Right].push_back( { 0 /* from ID */, 1 /* to ID */, 1  /* from offset */, 18 /* to offset */, 4  /* range */ } );
   stageDefs->ArenaPortalMap[Direction::Down].push_back(  { 0 /* from ID */, 2 /* to ID */, 42 /* from offset */, 9  /* to offset */, 34 /* range */ } );

   return stageDefs;
}
