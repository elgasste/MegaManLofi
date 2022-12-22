#pragma once

#include <memory>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IConsoleSprite;
   class IFrameRateProvider;

   class MenuSpriteGenerator
   {
   public:
      static ConsoleImage GeneratePlayerImage();
      static std::shared_ptr<IConsoleSprite> GenerateMenuCaratSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
