#include "ArenaSpriteGenerator.h"
#include "ArenaGenerationDefs.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

vector<int> ArenaSpriteGenerator::GenerateArenaSprites()
{
   vector<int> sprites;
   auto arenaTilesString = ArenaGenerationDefs::GetArenaTilesString();

   for ( int i = 0; i < (int)arenaTilesString.size(); i++ )
   {
      switch ( arenaTilesString[i] )
      {
         case 's':
            sprites.push_back( 0 ); // passable in any direction
            break;
         case 'p':
            sprites.push_back( 1 ); // passable in any direction but down
            break;
         case 'x':
            sprites.push_back( 2 ); // passable in any direction but up
            break;
         default:
            sprites.push_back( -1 );
            break;
      }
   }

   return sprites;
}

ConsoleSprite ArenaSpriteGenerator::GeneratePauseOverlaySprite()
{
   ConsoleSprite sprite;

   sprite.Width = 28;
   sprite.Height = 3;

   string content = "                            " \
                    "          (PAUSED)          " \
                    "                            ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      sprite.Pixels.push_back( { content[i], false, ConsoleColor::White, ConsoleColor::DarkMagenta } );
   }

   return sprite;
}
