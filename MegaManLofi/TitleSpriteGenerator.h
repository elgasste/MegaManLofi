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
   };
}
