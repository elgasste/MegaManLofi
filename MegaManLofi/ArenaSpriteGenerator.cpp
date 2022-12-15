#include "ArenaSpriteGenerator.h"
#include "ArenaGenerationDefs.h"

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

ConsoleImage ArenaSpriteGenerator::GenerateGetReadySprite()
{
   ConsoleImage sprite;

   sprite.Width = 10;
   sprite.Height = 1;

   string content = "GET READY!";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      sprite.Pixels.push_back( { content[i], false, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   return sprite;
}

ConsoleImage ArenaSpriteGenerator::GeneratePauseOverlaySprite()
{
   ConsoleImage sprite;

   sprite.Width = 28;
   sprite.Height = 3;

   string content =
      "                            " \
      "          (PAUSED)          " \
      "                            ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      sprite.Pixels.push_back( { content[i], false, ConsoleColor::White, ConsoleColor::DarkMagenta } );
   }

   return sprite;
}

ConsoleImage ArenaSpriteGenerator::GenerateGameOverSprite()
{
   ConsoleImage sprite;

   sprite.Width = 76;
   sprite.Height = 10;

   string content =
      " _______  _______  __   __  _______     _______  __   __  _______  ______   " \
      "|       ||   _   ||  |_|  ||       |   |       ||  | |  ||       ||    _ |  " \
      "|    ___||  |_|  ||       ||    ___|   |   _   ||  |_|  ||    ___||   | ||  " \
      "|   | __ |       ||       ||   |___    |  | |  ||       ||   |___ |   |_||_ " \
      "|   ||  ||       ||       ||    ___|   |  |_|  ||       ||    ___||    __  |" \
      "|   |_| ||   _   || ||_|| ||   |___    |       | |     | |   |___ |   |  | |" \
      "|_______||__| |__||_|   |_||_______|   |_______|  |___|  |_______||___|  |_|" \
      "                                                                            " \
      "                                                                            " \
      "                        press any button to continue                        ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      sprite.Pixels.push_back( { content[i], true, ConsoleColor::White, ConsoleColor::Black } );
   }

   return sprite;
}
