#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameRenderDefs;
   class IGameInputDefs;
   class EntityDefs;
   class PlayerDefs;
   class ArenaDefs;
   class PlayerPhysicsDefs;

   class GameDefs
   {
   public:
      std::shared_ptr<IGameRenderDefs> RenderDefs;
      std::shared_ptr<IGameInputDefs> InputDefs;
      std::shared_ptr<EntityDefs> EntityDefs;
      std::shared_ptr<PlayerDefs> PlayerDefs;
      std::shared_ptr<ArenaDefs> ArenaDefs;
      std::shared_ptr<PlayerPhysicsDefs> PlayerPhysicsDefs;
   };
}
