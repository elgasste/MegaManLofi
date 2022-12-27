#pragma once

#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class TitleImageGenerator
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
