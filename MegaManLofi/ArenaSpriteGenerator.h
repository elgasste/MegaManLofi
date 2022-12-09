#pragma once

#include <vector>

#include "ConsoleSprite.h"

namespace MegaManLofi
{
   class ArenaSpriteGenerator
   {
   public:
      static std::vector<int> GenerateArenaSprites();
      static ConsoleSprite GeneratePauseOverlaySprite();
   };
}
