#include "ArenaTileGenerator.h"
#include "ArenaGenerationDefs.h"

using namespace std;
using namespace MegaManLofi;

vector<ArenaTile> ArenaTileGenerator::GenerateArenaTiles()
{
   vector<ArenaTile> tiles;
   auto arenaTilesString = ArenaGenerationDefs::GetArenaTilesString();

   for ( int i = 0; i < (int)arenaTilesString.size(); i++ )
   {
      switch ( arenaTilesString[i] )
      {
         case 's':
            tiles.push_back( { false, false, false, false } );
            break;
         case 'p':
            tiles.push_back( { true, true, true, false } ); // down-passable only
            break;
         default:
            tiles.push_back( { true, true, true, true } );
            break;
      }
   }

   return tiles;
}
