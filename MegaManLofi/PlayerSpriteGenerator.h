#pragma once

#include <memory>
#include <map>

#include "ConsoleSprite.h"

namespace MegaManLofi
{
   enum class Direction;

   class PlayerSpriteGenerator
   {
   public:
      static std::map<Direction, ConsoleImage> GenerateStaticSpriteMap();
      static std::map<Direction, ConsoleImage> GenerateMovingSpriteMap();
      static std::shared_ptr<ConsoleSprite> GeneratePlayerThwipSprite();
      static std::shared_ptr<ConsoleSprite> GenerateExplosionParticleSprite();
   };
}
