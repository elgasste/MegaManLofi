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
      static std::shared_ptr<ConsoleSprite> GenerateThwipSprite();
      static std::shared_ptr<ConsoleSprite> GenerateThwipInTransitionSprite();
      static std::shared_ptr<ConsoleSprite> GenerateThwipOutTransitionSprite();
      static std::shared_ptr<ConsoleSprite> GenerateExplosionParticleSprite();

      static std::map<Direction, std::shared_ptr<ConsoleSprite>> GenerateStandingSpriteMap();
      static std::map<Direction, std::shared_ptr<ConsoleSprite>> GenerateWalkingSpriteMap();
      static std::map<Direction, std::shared_ptr<ConsoleSprite>> GenerateFallingSpriteMap();
   };
}
