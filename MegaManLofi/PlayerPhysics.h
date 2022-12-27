#pragma once

#include "IPlayerPhysics.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class PlayerPhysicsDefs;

   class PlayerPhysics : public IPlayerPhysics
   {
   public:
      PlayerPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
                     const std::shared_ptr<PlayerPhysicsDefs> physicsDefs );

      void AssignTo( const std::shared_ptr<IPlayer> player ) override;
      void Tick() override;

      void PointTo( Direction direction ) const override;
      void PushTo( Direction direction ) const override;
      void Jump() override;
      void ExtendJump() override;

   private:
      void ApplyFriction() const;
      void ApplyGravity() const;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<PlayerPhysicsDefs> _physicsDefs;
      std::shared_ptr<IPlayer> _player;

      long long _lastExtendJumpFrame;
   };
}
