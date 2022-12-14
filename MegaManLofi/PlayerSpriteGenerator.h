#pragma once

#include <map>

#include "ConsoleSprite.h"

namespace MegaManLofi
{
   enum class Direction;

   class PlayerSpriteGenerator
   {
   public:
      static std::map<Direction, ConsoleSprite> GenerateStaticSpriteMap();
      static std::map<Direction, ConsoleSprite> GenerateMovingSpriteMap();
      static ConsoleSprite GeneratePlayerThwipSprite();
   };
}
