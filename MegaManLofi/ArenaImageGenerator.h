#pragma once

#include <memory>
#include <vector>
#include <map>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IConsoleSprite;
   class IFrameRateProvider;

   class ArenaImageGenerator
   {
   public:
      static std::vector<int> GenerateArenaTiles();
      static ConsoleImage GeneratePauseOverlayImage();
      static ConsoleImage GenerateGameOverImage();
   };
}
