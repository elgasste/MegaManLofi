#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameRenderConfig;
   class IGameInputDefs;
   class PlayerDefs;
   class ArenaDefs;
   class PlayerPhysicsConfig;

   class GameConfig
   {
   public:
      int FramesPerSecond = 0;

      std::shared_ptr<IGameRenderConfig> RenderConfig;
      std::shared_ptr<IGameInputDefs> InputDefs;
      std::shared_ptr<PlayerDefs> PlayerDefs;
      std::shared_ptr<ArenaDefs> ArenaDefs;
      std::shared_ptr<PlayerPhysicsConfig> PlayerPhysicsConfig;
   };
}
