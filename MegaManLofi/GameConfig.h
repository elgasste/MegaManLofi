#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameRenderConfig;
   class IGameInputConfig;
   class PlayerConfig;
   class ArenaConfig;
   enum class Direction;

   class GameConfig
   {
   public:
      int FramesPerSecond = 0;

      std::shared_ptr<IGameRenderConfig> RenderConfig;
      std::shared_ptr<IGameInputConfig> InputConfig;
      std::shared_ptr<PlayerConfig> PlayerConfig;
      std::shared_ptr<ArenaConfig> ArenaConfig;
   };
}
