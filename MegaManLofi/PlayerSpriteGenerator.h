#pragma once

#include <map>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   enum class Direction;

   class PlayerSpriteGenerator
   {
   public:
      static std::map<Direction, ConsoleImage> GenerateStaticSpriteMap();
      static std::map<Direction, ConsoleImage> GenerateMovingSpriteMap();
      static ConsoleImage GeneratePlayerThwipSprite();
   };
}
