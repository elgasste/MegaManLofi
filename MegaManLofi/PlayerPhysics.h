#pragma once

#include <memory>

#include "IPlayerPhysics.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class IPlayer;
   class PlayerPhysicsConfig;

   class PlayerPhysics : public IPlayerPhysics
   {
   public:
      PlayerPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
                     const std::shared_ptr<IPlayer> player,
                     const std::shared_ptr<PlayerPhysicsConfig> config );

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
      const std::shared_ptr<IPlayer> _player;
      const std::shared_ptr<PlayerPhysicsConfig> _config;

      long long _lastExtendJumpFrame;
      double _elapsedJumpExtensionSeconds;
   };
}
