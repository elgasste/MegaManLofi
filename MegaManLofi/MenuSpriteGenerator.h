#pragma once

#include <memory>

namespace MegaManLofi
{
   class IConsoleSprite;
   class IFrameRateProvider;

   class MenuSpriteGenerator
   {
   public:
      static std::shared_ptr<IConsoleSprite> GenerateMenuCaratSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
