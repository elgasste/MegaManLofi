#include "ArenaConsoleSpriteGenerator.h"
#include "ArenaGenerationDefs.h"

using namespace std;
using namespace MegaManLofi;

vector<int> ArenaConsoleSpriteGenerator::GenerateArenaSprites()
{
   vector<int> sprites;
   auto arenaTilesString = ArenaGenerationDefs::GetArenaTilesString();

   for ( int i = 0; i < (int)arenaTilesString.size(); i++ )
   {
      switch ( arenaTilesString[i] )
      {
         case 's':
            sprites.push_back( 0 ); // non-passable in any direction
            break;
         case 'p':
            sprites.push_back( 1 ); // only down-passable
            break;
         default:
            sprites.push_back( -1 );
            break;
      }
   }

   return sprites;
}
