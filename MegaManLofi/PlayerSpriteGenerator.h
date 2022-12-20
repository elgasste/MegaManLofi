#pragma once

#include <memory>
#include <map>

namespace MegaManLofi
{
   class IConsoleSprite;
   enum class Direction;

   class PlayerSpriteGenerator
   {
   public:
      static std::shared_ptr<IConsoleSprite> GenerateThwipSprite();
      static std::shared_ptr<IConsoleSprite> GenerateThwipInTransitionSprite();
      static std::shared_ptr<IConsoleSprite> GenerateThwipOutTransitionSprite();
      static std::shared_ptr<IConsoleSprite> GenerateExplosionParticleSprite();

      static std::map<Direction, std::shared_ptr<IConsoleSprite>> GenerateStandingSpriteMap();
      static std::map<Direction, std::shared_ptr<IConsoleSprite>> GenerateWalkingSpriteMap();
      static std::map<Direction, std::shared_ptr<IConsoleSprite>> GenerateFallingSpriteMap();
   };
}
