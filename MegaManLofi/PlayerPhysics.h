#pragma once

#include "IPlayerPhysics.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class PlayerConfig;

   class PlayerPhysics : public IPlayerPhysics
   {
   public:
      PlayerPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
                     const std::shared_ptr<PlayerConfig> playerConfig );

      void ApplyFriction( const std::shared_ptr<IPlayer> player ) const override;
      void ApplyGravity( const std::shared_ptr<IPlayer> player ) const override;
      void Push( const std::shared_ptr<IPlayer> player, Direction direction ) const override;
      void Jump( const std::shared_ptr<IPlayer> player ) const override;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<PlayerConfig> _playerConfig;
   };
}
