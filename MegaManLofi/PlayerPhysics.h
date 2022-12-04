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

      void ApplyFriction() const override;
      void ApplyGravity() const override;
      void Point( Direction direction ) const override;
      void Push( Direction direction ) const override;
      void Jump() const override;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IPlayer> _player;
      const std::shared_ptr<PlayerPhysicsConfig> _config;
   };
}
