#pragma once

#include <memory>

#include "IPlayerPhysics.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class IPlayer;
   class PlayerConfig;

   class PlayerPhysics : public IPlayerPhysics
   {
   public:
      PlayerPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
                     const std::shared_ptr<IPlayer> player,
                     const std::shared_ptr<PlayerConfig> playerConfig );

      void ApplyFriction() const override;
      void ApplyGravity() const override;
      void Push( Direction direction ) const override;
      void Jump() const override;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IPlayer> _player;
      const std::shared_ptr<PlayerConfig> _playerConfig;
   };
}
