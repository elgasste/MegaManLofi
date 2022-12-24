#include "ArenaDefsGenerator.h"
#include "ArenaDefs.h"
#include "ArenaTileGenerator.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<ArenaDefs> ArenaDefsGenerator::GenerateArenaDefs()
{
   auto arenaDefs = make_shared<ArenaDefs>();

   // this results in a 4560 x 2340 unit viewport, which translates super well to a 120 x 30 character console
   arenaDefs->DefaultTileWidth = 38;
   arenaDefs->DefaultTileHeight = 78;

   arenaDefs->DefaultHorizontalTiles = 360;
   arenaDefs->DefaultVerticalTiles = 60;

   arenaDefs->DefaultTiles = ArenaTileGenerator::GenerateArenaTiles();

   arenaDefs->DefaultPlayerPosition = { arenaDefs->DefaultTileWidth * 8, arenaDefs->DefaultTileHeight * 6 };

   return arenaDefs;
}
