#pragma once

#include <memory>
#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IConsoleSprite;

   class ArenaSpriteGenerator
   {
   public:
      static std::vector<int> GenerateArenaTiles();
      static std::shared_ptr<IConsoleSprite> GenerateGetReadySprite();
      static ConsoleImage GeneratePauseOverlayImage();
      static ConsoleImage GenerateGameOverImage();
   };
}
