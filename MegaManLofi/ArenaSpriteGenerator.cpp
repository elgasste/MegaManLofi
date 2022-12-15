#include "ArenaSpriteGenerator.h"
#include "ArenaGenerationDefs.h"
#include "ConsoleSprite.h"

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

shared_ptr<ConsoleSprite> ArenaSpriteGenerator::GenerateGetReadySprite()
{
   auto getReadySprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .25 ) );

   ConsoleImage getReadyImage = { 10, 1 };
   string message = "GET READY!";
   for ( int i = 0; i < (int)message.size(); i++ )
   {
      getReadyImage.Pixels.push_back( { message[i], false, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   getReadySprite->AddImage( getReadyImage );
   getReadySprite->AddImage( { 0, 0 } );

   return getReadySprite;
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
