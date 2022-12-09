#include <string>

#include "TitleSpriteGenerator.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

ConsoleSprite TitleSpriteGenerator::GenerateTitleTextSprite()
{
   ConsoleSprite sprite;

   sprite.Width = 74;
   sprite.Height = 7;

   string content =
      " __   __  _______  _______  _______    __   __  _______  __    _  ___     " \
      "|  |_|  ||       ||       ||   _   |  |  |_|  ||   _   ||  |  | ||   |    " \
      "|       ||    ___||    ___||  |_|  |  |       ||  |_|  ||   |_| ||___|    " \
      "|       ||   |___ |   | __ |       |  |       ||       ||       | ___     " \
      "|       ||    ___||   ||  ||       |  |       ||       ||  _    ||   |    " \
      "| ||_|| ||   |___ |   |_| ||   _   |  | ||_|| ||   _   || | |   ||___|    " \
      "|_|   |_||_______||_______||__| |__|  |_|   |_||__| |__||_|  |__|         ";

   string lofiContent =
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

ConsoleSprite TitleSpriteGenerator::GenerateTitleSubTextSprite()
{
   ConsoleSprite sprite;

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
      sprite.Pixels.push_back( { content[i], true, ConsoleColor::Blue, ConsoleColor::Black } );
   }

   return sprite;
}
