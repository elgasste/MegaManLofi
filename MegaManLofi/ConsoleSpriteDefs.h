#pragma once

#include <memory>
#include <map>

#include "Direction.h"

namespace MegaManLofi
{
   class IConsoleSprite;

   class ConsoleSpriteDefs
   {
   public:
      std::map<Direction, std::shared_ptr<IConsoleSprite>> PlayerStandingSpriteMap;
      std::map<Direction, std::shared_ptr<IConsoleSprite>> PlayerWalkingSpriteMap;
      std::map<Direction, std::shared_ptr<IConsoleSprite>> PlayerAirborneSpriteMap;

      std::shared_ptr<IConsoleSprite> PlayerThwipSprite;
      std::shared_ptr<IConsoleSprite> PlayerThwipInTransitionSprite;
      std::shared_ptr<IConsoleSprite> PlayerThwipOutTransitionSprite;

      std::shared_ptr<IConsoleSprite> PlayerExplosionParticleSprite;
      std::shared_ptr<IConsoleSprite> StageGetReadySprite;
      std::shared_ptr<IConsoleSprite> MenuCaratSprite;

      std::map<int, std::shared_ptr<IConsoleSprite>> EntitySpriteMap;
   };
}
