#pragma once

#include <vector>

#include "ConsoleSprite.h"

namespace MegaManLofi
{
   class TitleSpriteGenerator
   {
   public:
      static ConsoleSprite GenerateTitleTextSprite();
      static ConsoleSprite GenerateTitleSubTextSprite();
      static ConsoleSprite GeneratePlayerSprite();
      static ConsoleSprite GenerateBuildingSprite();
      static ConsoleSprite GenerateStartMessageSprite();
      static ConsoleSprite GenerateStarSprite();
   };
}
