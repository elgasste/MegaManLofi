#include <string>

#include "TitleSpriteGenerator.h"

using namespace std;
using namespace MegaManLofi;

ConsoleImage TitleSpriteGenerator::GenerateTitleTextSprite()
{
   ConsoleImage sprite;

   sprite.Width = 75;
   sprite.Height = 7;

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
      sprite.Pixels.push_back( { content[i], true, ConsoleColor::Blue, ConsoleColor::Black } );
   }

   return sprite;
}

ConsoleImage TitleSpriteGenerator::GenerateTitleSubTextSprite()
{
   ConsoleImage sprite;

   sprite.Width = 32;
   sprite.Height = 7;

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
      sprite.Pixels.push_back( { content[i], true, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   return sprite;
}

ConsoleImage TitleSpriteGenerator::GeneratePlayerSprite()
{
   ConsoleImage sprite;

   sprite.Width = 3;
   sprite.Height = 3;

   string content =
      " O " \
      "/|\\" \
      "/ \\";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      sprite.Pixels.push_back( { content[i], true, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   return sprite;
}

ConsoleImage TitleSpriteGenerator::GenerateBuildingSprite()
{
   ConsoleImage sprite;

   sprite.Width = 30;
   sprite.Height = 12;

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
      sprite.Pixels.push_back( { content[i], false, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   }

   return sprite;
}

ConsoleImage TitleSpriteGenerator::GenerateStartMessageSprite()
{
   ConsoleImage sprite;

   sprite.Width = 19;
   sprite.Height = 2;

   string content =
      "Press any button to" \
      "  start the game!  ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      sprite.Pixels.push_back( { content[i], true, ConsoleColor::White, ConsoleColor::Black } );
   }

   return sprite;
}

ConsoleImage TitleSpriteGenerator::GenerateStarSprite()
{
   ConsoleImage sprite;

   sprite.Width = 1;
   sprite.Height = 1;
   sprite.Pixels.push_back( { '.', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );

   return sprite;
}
