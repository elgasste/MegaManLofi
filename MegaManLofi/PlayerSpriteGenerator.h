#pragma once

#include <map>

namespace MegaManLofi
{
   struct ConsoleSprite;
   enum class Direction;

   class PlayerSpriteGenerator
   {
   public:
      static std::map<Direction, ConsoleSprite> GenerateStaticSpriteMap();
      static std::map<Direction, ConsoleSprite> GenerateMovingSpriteMap();
   };
}
