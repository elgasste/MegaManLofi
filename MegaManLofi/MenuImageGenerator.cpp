#include <string>

#include "MenuImageGenerator.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

ConsoleImage MenuImageGenerator::GeneratePlayerImage()
{
   ConsoleImage image;

   image.Width = 16;
   image.Height = 9;

   string content =
      " _    ___       " \
      "( , .' `.`      " \
      " `.`()'o o|__ _ " \
      "   `.`._C,'__)_)" \
      "     |    L     " \
      "     |  __,.    " \
      "     |_(  L__b  " \
      "     `| |       " \
      "      '_'       ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], true, ConsoleColor::White, ConsoleColor::Black } );
   }

   return image;
}
