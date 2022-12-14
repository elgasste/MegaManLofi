#pragma once

#include "ConsoleColor.h"

namespace MegaManLofi
{
   struct ConsolePixel
   {
      char Value = '\0';
      bool HasTransparency = true;
      ConsoleColor ForegroundColor = (ConsoleColor)0;
      ConsoleColor BackgroundColor = (ConsoleColor)0;
   };
}
