#pragma once

#include <memory>

namespace MegaManLofi
{
   class FrameRateDefs;
   class IGameRenderDefs;
   class IGameInputDefs;
   class PlayerDefs;
   class ArenaDefs;
   class PlayerPhysicsDefs;

   class GameDefs
   {
   public:
      std::shared_ptr<FrameRateDefs> FrameRateDefs;
      std::shared_ptr<IGameRenderDefs> RenderDefs;
      std::shared_ptr<IGameInputDefs> InputDefs;
      std::shared_ptr<PlayerDefs> PlayerDefs;
      std::shared_ptr<ArenaDefs> ArenaDefs;
      std::shared_ptr<PlayerPhysicsDefs> PlayerPhysicsDefs;
   };
}
