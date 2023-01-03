#pragma once

#include <memory>
#include <map>

#include "Direction.h"

namespace MegaManLofi
{
   class ConsoleSprite;
   class IEntityConsoleSprite;

   class ConsoleSpriteDefs
   {
   public:
      std::shared_ptr<ConsoleSprite> PlayerThwipSprite;
      std::shared_ptr<ConsoleSprite> PlayerThwipInTransitionSprite;
      std::shared_ptr<ConsoleSprite> PlayerThwipOutTransitionSprite;

      std::shared_ptr<ConsoleSprite> PlayerExplosionParticleSprite;
      std::shared_ptr<ConsoleSprite> StageGetReadySprite;
      std::shared_ptr<ConsoleSprite> MenuCaratSprite;

      std::map<int, std::shared_ptr<IEntityConsoleSprite>> EntitySpriteMap;
   };
}
