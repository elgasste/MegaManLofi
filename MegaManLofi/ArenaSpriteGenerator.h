#pragma once

#include <memory>
#include <vector>
#include <map>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IConsoleSprite;
   class IFrameRateProvider;

   class ArenaSpriteGenerator
   {
   public:
      static std::vector<int> GenerateArenaTiles();
      static std::shared_ptr<IConsoleSprite> GenerateGetReadySprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static ConsoleImage GeneratePauseOverlayImage();
      static ConsoleImage GenerateGameOverImage();
      static std::map<int, std::shared_ptr<IConsoleSprite>> GenerateEntitySpriteMap( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
