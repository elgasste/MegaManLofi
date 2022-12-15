#pragma once

#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class TitleSpriteGenerator
   {
   public:
      static ConsoleImage GenerateTitleTextSprite();
      static ConsoleImage GenerateTitleSubTextSprite();
      static ConsoleImage GeneratePlayerSprite();
      static ConsoleImage GenerateBuildingSprite();
      static ConsoleImage GenerateStartMessageSprite();
      static ConsoleImage GenerateStarSprite();
   };
}
