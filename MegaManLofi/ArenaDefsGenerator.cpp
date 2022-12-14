#include "ArenaDefsGenerator.h"
#include "ArenaTileGenerator.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"

using namespace std;
using namespace MegaManLofi;

map<int, shared_ptr<ArenaDefs>> ArenaDefsGenerator::GenerateArenaDefsMap( const shared_ptr<WorldDefs> worldDefs )
{
   map<int, shared_ptr<ArenaDefs>> arenaDefsMap;

   /********************* ARENA 1 *********************/

   arenaDefsMap[0] = make_shared<ArenaDefs>();
   arenaDefsMap[0]->ArenaId = 0;
   arenaDefsMap[0]->HorizontalTiles = 360;
   arenaDefsMap[0]->VerticalTiles = 60;
   arenaDefsMap[0]->Tiles = ArenaTileGenerator::GenerateArenaTiles( 0 );
   arenaDefsMap[0]->PlayerStartPosition = { worldDefs->TileWidth * 8, worldDefs->TileHeight * 6 };

   // small health drop at 4, 52
   arenaDefsMap[0]->SpawnPoints.push_back( SpawnPoint() );
   arenaDefsMap[0]->SpawnPoints[0].EntityMetaId = 2;
   arenaDefsMap[0]->SpawnPoints[0].ArenaPosition = { worldDefs->TileWidth * 4, worldDefs->TileHeight * 52 };
   arenaDefsMap[0]->SpawnPoints[0].IsBoundToUniqueId = true;

   // large health drop at 306, 38
   arenaDefsMap[0]->SpawnPoints.push_back( SpawnPoint() );
   arenaDefsMap[0]->SpawnPoints[1].EntityMetaId = 3;
   arenaDefsMap[0]->SpawnPoints[1].ArenaPosition = { worldDefs->TileWidth * 306, worldDefs->TileHeight * 38 };
   arenaDefsMap[0]->SpawnPoints[1].IsBoundToUniqueId = true;

   /********************* ARENA 2 *********************/

   arenaDefsMap[1] = make_shared<ArenaDefs>();
   arenaDefsMap[1]->ArenaId = 1;
   arenaDefsMap[1]->HorizontalTiles = 120;
   arenaDefsMap[1]->VerticalTiles = 28;
   arenaDefsMap[1]->Tiles = ArenaTileGenerator::GenerateArenaTiles( 1 );
   arenaDefsMap[1]->PlayerStartPosition = { worldDefs->TileWidth * 2, worldDefs->TileHeight * 19 };

   // extra life at 112, 24
   arenaDefsMap[1]->SpawnPoints.push_back( SpawnPoint() );
   arenaDefsMap[1]->SpawnPoints[0].EntityMetaId = 4;
   arenaDefsMap[1]->SpawnPoints[0].ArenaPosition = { worldDefs->TileWidth * 112, worldDefs->TileHeight * 24 };
   arenaDefsMap[1]->SpawnPoints[0].IsBoundToUniqueId = true;

   /********************* ARENA 3 *********************/

   arenaDefsMap[2] = make_shared<ArenaDefs>();
   arenaDefsMap[2]->ArenaId = 2;
   arenaDefsMap[2]->HorizontalTiles = 120;
   arenaDefsMap[2]->VerticalTiles = 60;
   arenaDefsMap[2]->Tiles = ArenaTileGenerator::GenerateArenaTiles( 2 );
   arenaDefsMap[2]->PlayerStartPosition = { worldDefs->TileWidth * 28, worldDefs->TileHeight * 2 };

   // large health drop at 112, 6
   arenaDefsMap[2]->SpawnPoints.push_back( SpawnPoint() );
   arenaDefsMap[2]->SpawnPoints[0].EntityMetaId = 3;
   arenaDefsMap[2]->SpawnPoints[0].ArenaPosition = { worldDefs->TileWidth * 112, worldDefs->TileHeight * 6 };
   arenaDefsMap[2]->SpawnPoints[0].IsBoundToUniqueId = true;

   return arenaDefsMap;
}
