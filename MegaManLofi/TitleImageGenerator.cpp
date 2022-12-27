#include <string>

#include "TitleImageGenerator.h"

using namespace std;
using namespace MegaManLofi;

ConsoleImage TitleImageGenerator::GenerateTitleTextImage()
{
   ConsoleImage image;

   image.Width = 75;
   image.Height = 7;

   string content =
      " __   __  _______  _______  _______     __   __  _______  __    _  ___     " \
      "|  |_|  ||       ||       ||   _   |   |  |_|  ||   _   ||  |  | ||   |    " \
      "|       ||    ___||    ___||  |_|  |   |       ||  |_|  ||   |_| ||___|    " \
      "|       ||   |___ |   | __ |       |   |       ||       ||       | ___     " \
      "|       ||    ___||   ||  ||       |   |       ||       ||  _    ||   |    " \
      "| ||_|| ||   |___ |   |_| ||   _   |   | ||_|| ||   _   || | |   ||___|    " \
      "|_|   |_||_______||_______||__| |__|   |_|   |_||__| |__||_|  |__|         ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], true, ConsoleColor::Blue, ConsoleColor::Black } );
   }

   return image;
}

ConsoleImage TitleImageGenerator::GenerateTitleSubTextImage()
{
   ConsoleImage image;

   image.Width = 32;
   image.Height = 7;

   string content =
      " ___      _______  _______  ___ " \
      "|   |    |       ||       ||   |" \
      "|   |    |   _   ||    ___||   |" \
      "|   |    |  | |  ||   |___ |   |" \
      "|   |___ |  |_|  ||    ___||   |" \
      "|       ||       ||   |    |   |" \
      "|_______||_______||___|    |___|";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], true, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   return image;
}

ConsoleImage TitleImageGenerator::GeneratePlayerImage()
{
   ConsoleImage image;

   image.Width = 3;
   image.Height = 3;

   string content =
      " O " \
      "/|\\" \
      "/ \\";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], true, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   return image;
}

ConsoleImage TitleImageGenerator::GenerateBuildingImage()
{
   ConsoleImage image;

   image.Width = 30;
   image.Height = 12;

   string content =
      "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.," \
      "|,.,.,.,|,.,.,.,|,.,.,.,|,.,.,";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], false, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   }

   return image;
}

ConsoleImage TitleImageGenerator::GenerateStartMessageImage()
{
   ConsoleImage image;

   image.Width = 19;
   image.Height = 2;

   string content =
      "Press any button to" \
      "  start the game!  ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], true, ConsoleColor::White, ConsoleColor::Black } );
   }

   return image;
}

ConsoleImage TitleImageGenerator::GenerateStarImage()
{
   ConsoleImage image;

   image.Width = 1;
   image.Height = 1;
   image.Pixels.push_back( { '.', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );

   return image;
}
