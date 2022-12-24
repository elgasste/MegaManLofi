#include "ArenaTileGenerator.h"
#include "ArenaGenerationStrings.h"

using namespace std;
using namespace MegaManLofi;

vector<ArenaTile> ArenaTileGenerator::GenerateArenaTiles()
{
   vector<ArenaTile> tiles;
   auto arenaTilesString = ArenaGenerationStrings::GetArenaTilesString();

   for ( int i = 0; i < (int)arenaTilesString.size(); i++ )
   {
      switch ( arenaTilesString[i] )
      {
         case 's':
            tiles.push_back( { false, false, false, false, false } ); // fully-blocking, no death
            break;
         case 'p':
            tiles.push_back( { true, true, true, false, false } ); // down-blocking only, no death
            break;
         case 'x':
            tiles.push_back( { false, false, false, true, true } ); // up-blocking only, causes death
            break;
         default:
            tiles.push_back( { true, true, true, true } ); // non-blocking, no death
            break;
      }
   }

   return tiles;
}
