#pragma once

#include <memory>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class ConsoleSprite;
   class IFrameRateProvider;

   class MenuImageGenerator
   {
   public:
      static ConsoleImage GeneratePlayerImage();
   };
}
