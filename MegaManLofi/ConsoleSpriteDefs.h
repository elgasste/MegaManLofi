#pragma once

#include <memory>
#include <map>

#include "Direction.h"

namespace MegaManLofi
{
   class IConsoleSprite;
   class IEntityConsoleSprite;

   class ConsoleSpriteDefs
   {
   public:
      std::shared_ptr<IConsoleSprite> PlayerThwipSprite;
      std::shared_ptr<IConsoleSprite> PlayerThwipInTransitionSprite;
      std::shared_ptr<IConsoleSprite> PlayerThwipOutTransitionSprite;

      std::shared_ptr<IConsoleSprite> PlayerExplosionParticleSprite;
      std::shared_ptr<IConsoleSprite> StageGetReadySprite;
      std::shared_ptr<IConsoleSprite> MenuCaratSprite;

      std::map<int, std::shared_ptr<IEntityConsoleSprite>> EntitySpriteMap;
   };
}
