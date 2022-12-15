#pragma once

#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class ArenaSpriteGenerator
   {
   public:
      static std::vector<int> GenerateArenaSprites();
      static ConsoleImage GenerateGetReadySprite();
      static ConsoleImage GeneratePauseOverlaySprite();
      static ConsoleImage GenerateGameOverSprite();
   };
}
