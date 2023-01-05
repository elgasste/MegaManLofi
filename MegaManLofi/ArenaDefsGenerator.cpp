#include "ArenaDefsGenerator.h"
#include "ArenaTileGenerator.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"

using namespace std;
using namespace MegaManLofi;

map<int, shared_ptr<ArenaDefs>> ArenaDefsGenerator::GenerateArenaDefsMap( const shared_ptr<WorldDefs> worldDefs )
{
   map<int, shared_ptr<ArenaDefs>> arenaDefsMap;

   arenaDefsMap[0] = make_shared<ArenaDefs>();
   arenaDefsMap[0]->ArenaId = 0;
   arenaDefsMap[0]->HorizontalTiles = 360;
   arenaDefsMap[0]->VerticalTiles = 60;
   arenaDefsMap[0]->Tiles = ArenaTileGenerator::GenerateArenaTiles( 0 );
   arenaDefsMap[0]->PlayerStartPosition = { worldDefs->TileWidth * 8, worldDefs->TileHeight * 6 };

   arenaDefsMap[1] = make_shared<ArenaDefs>();
   arenaDefsMap[1]->ArenaId = 1;
   arenaDefsMap[1]->HorizontalTiles = 120;
   arenaDefsMap[1]->VerticalTiles = 30;
   arenaDefsMap[1]->Tiles = ArenaTileGenerator::GenerateArenaTiles( 1 );
   arenaDefsMap[1]->PlayerStartPosition = { worldDefs->TileWidth * 2, worldDefs->TileHeight * 19 };

   arenaDefsMap[2] = make_shared<ArenaDefs>();
   arenaDefsMap[2]->ArenaId = 2;
   arenaDefsMap[2]->HorizontalTiles = 120;
   arenaDefsMap[2]->VerticalTiles = 60;
   arenaDefsMap[2]->Tiles = ArenaTileGenerator::GenerateArenaTiles( 2 );
   arenaDefsMap[2]->PlayerStartPosition = { worldDefs->TileWidth * 28, worldDefs->TileHeight * 2 };

   return arenaDefsMap;
}
