#include "ArenaSpriteGenerator.h"
#include "ArenaGenerationDefs.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

vector<int> ArenaSpriteGenerator::GenerateArenaTiles()
{
   vector<int> imageIds;
   auto arenaTilesString = ArenaGenerationDefs::GetArenaTilesString();

   for ( int i = 0; i < (int)arenaTilesString.size(); i++ )
   {
      switch ( arenaTilesString[i] )
      {
         case 's':
            imageIds.push_back( 0 ); // passable in any direction
            break;
         case 'p':
            imageIds.push_back( 1 ); // passable in any direction but down
            break;
         case 'x':
            imageIds.push_back( 2 ); // passable in any direction but up
            break;
         default:
            imageIds.push_back( -1 );
            break;
      }
   }

   return imageIds;
}

shared_ptr<ConsoleSprite> ArenaSpriteGenerator::GenerateGetReadySprite()
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .25 ) );

   ConsoleImage getReadyImage = { 10, 1 };
   string message = "GET READY!";
   for ( int i = 0; i < (int)message.size(); i++ )
   {
      getReadyImage.Pixels.push_back( { message[i], false, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   sprite->AddImage( getReadyImage );
   sprite->AddImage( { 0, 0 } );

   return sprite;
}

ConsoleImage ArenaSpriteGenerator::GeneratePauseOverlayImage()
{
   ConsoleImage image;

   image.Width = 28;
   image.Height = 3;

   string content =
      "                            " \
      "          (PAUSED)          " \
      "                            ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], false, ConsoleColor::White, ConsoleColor::DarkMagenta } );
   }

   return image;
}

ConsoleImage ArenaSpriteGenerator::GenerateGameOverImage()
{
   ConsoleImage image;

   image.Width = 76;
   image.Height = 10;

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
      image.Pixels.push_back( { content[i], true, ConsoleColor::White, ConsoleColor::Black } );
   }

   return image;
}
