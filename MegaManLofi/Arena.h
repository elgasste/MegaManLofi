#pragma once

#include "ReadOnlyArena.h"

namespace MegaManLofi
{
   class ArenaDefs;
   class IGameEventAggregator;

   class Arena : public ReadOnlyArena
   {
   public:
      Arena() { }
      Arena( const std::shared_ptr<ArenaDefs> arenaDefs,
             const std::shared_ptr<IGameEventAggregator> eventAggregator );

      virtual void Reset();

      virtual void SetPlayerEntity( const std::shared_ptr<Entity> playerEntity );
      virtual void SetActiveRegion( Quad<float> region ) { _activeRegion = region; }
      virtual void AddEntity( const std::shared_ptr<Entity> entity );
      virtual void RemoveEntity( const std::shared_ptr<Entity> entity );

   private:
      const std::shared_ptr<ArenaDefs> _arenaDefs;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
   };
}
