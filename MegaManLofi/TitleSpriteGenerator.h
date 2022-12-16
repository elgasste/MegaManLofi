#pragma once

#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class TitleSpriteGenerator
   {
   public:
      static ConsoleImage GenerateTitleTextImage();
      static ConsoleImage GenerateTitleSubTextImage();
      static ConsoleImage GeneratePlayerImage();
      static ConsoleImage GenerateBuildingImage();
      static ConsoleImage GenerateStartMessageImage();
      static ConsoleImage GenerateStarImage();
   };
}
