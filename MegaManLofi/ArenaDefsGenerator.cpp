#include "ArenaDefsGenerator.h"
#include "ArenaTileGenerator.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<ArenaDefs> ArenaDefsGenerator::GenerateArenaDefs( const shared_ptr<WorldDefs> worldDefs )
{
   auto arenaDefs = make_shared<ArenaDefs>();

   arenaDefs->ArenaId = 0;

   arenaDefs->HorizontalTiles = 360;
   arenaDefs->VerticalTiles = 60;

   arenaDefs->Tiles = ArenaTileGenerator::GenerateArenaTiles();

   arenaDefs->PlayerStartPosition = { worldDefs->TileWidth * 8, worldDefs->TileHeight * 6 };

   return arenaDefs;
}
