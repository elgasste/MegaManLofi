#pragma once

#include <vector>

#include "ConsolePixel.h"

namespace MegaManLofi
{
   struct ConsoleImage
   {
      short Width = 0;
      short Height = 0;

      std::vector<ConsolePixel> Pixels;
   };
}
