#include "WorldDefsGenerator.h"
#include "WorldDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<WorldDefs> WorldDefsGenerator::GenerateWorldDefs()
{
   auto worldDefs = make_shared<WorldDefs>();

   // this results in a 4,560 x 2,340 unit viewport, which translates super well to a 120 x 30 character console
   worldDefs->TileWidth = 38;
   worldDefs->TileHeight = 78;

   // this conveniently matches the console viewport size
   worldDefs->ActiveRegionWidth = 120 * worldDefs->TileWidth;
   worldDefs->ActiveRegionHeight = 28 * worldDefs->TileHeight;

   return worldDefs;
}
