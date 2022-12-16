#pragma once

#include <memory>
#include <vector>

#include "ConsoleSprite.h"

namespace MegaManLofi
{
   class ArenaSpriteGenerator
   {
   public:
      static std::vector<int> GenerateArenaTiles();
      static std::shared_ptr<ConsoleSprite> GenerateGetReadySprite();
      static ConsoleImage GeneratePauseOverlayImage();
      static ConsoleImage GenerateGameOverImage();
   };
}
