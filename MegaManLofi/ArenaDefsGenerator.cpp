#include "ArenaDefsGenerator.h"
#include "ArenaTileGenerator.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"

using namespace std;
using namespace MegaManLofi;

map<int, shared_ptr<ArenaDefs>> ArenaDefsGenerator::GenerateArenaDefsMap( const shared_ptr<WorldDefs> worldDefs )
{
   map<int, shared_ptr<ArenaDefs>> arenaDefsMap;

   int arenaId = 0;
   arenaDefsMap[arenaId] = make_shared<ArenaDefs>();
   arenaDefsMap[arenaId]->ArenaId = arenaId;
   arenaDefsMap[arenaId]->HorizontalTiles = 360;
   arenaDefsMap[arenaId]->VerticalTiles = 60;
   arenaDefsMap[arenaId]->Tiles = ArenaTileGenerator::GenerateArenaTiles();
   arenaDefsMap[arenaId]->PlayerStartPosition = { worldDefs->TileWidth * 8, worldDefs->TileHeight * 6 };

   // TODO: increment arena ID and add another arena

   return arenaDefsMap;
}
