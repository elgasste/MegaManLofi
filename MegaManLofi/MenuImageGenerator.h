#pragma once

#include <memory>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IConsoleSprite;
   class IFrameRateProvider;

   class MenuImageGenerator
   {
   public:
      static ConsoleImage GeneratePlayerImage();
   };
}
