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

   stageDefs->ArenaPortalMap[Direction::Right].push_back( shared_ptr<ArenaPortal>( new ArenaPortal( { 0 /* from ID */, 1 /* to ID */, 1  /* from offset */, 18 /* to offset */, 4  /* range */ } ) ) );
   stageDefs->ArenaPortalMap[Direction::Left].push_back(  shared_ptr<ArenaPortal>( new ArenaPortal( { 1 /* from ID */, 0 /* to ID */, 18 /* from offset */, 1  /* to offset */, 4  /* range */ } ) ) );
   stageDefs->ArenaPortalMap[Direction::Down].push_back(  shared_ptr<ArenaPortal>( new ArenaPortal( { 0 /* from ID */, 2 /* to ID */, 42 /* from offset */, 9  /* to offset */, 34 /* range */ } ) ) );
   stageDefs->ArenaPortalMap[Direction::Up].push_back(    shared_ptr<ArenaPortal>( new ArenaPortal( { 2 /* from ID */, 0 /* to ID */, 9  /* from offset */, 42 /* to offset */, 34 /* range */ } ) ) );

   return stageDefs;
}
