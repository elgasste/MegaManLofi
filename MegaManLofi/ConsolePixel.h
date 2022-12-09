#pragma once

namespace MegaManLofi
{
   enum class ConsoleColor;

   struct ConsolePixel
   {
      char Value = '\0';
      bool HasTransparency = true;
      ConsoleColor ForegroundColor = (ConsoleColor)0;
      ConsoleColor BackgroundColor = (ConsoleColor)0;
   };
}
