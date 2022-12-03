#pragma once

#include "IPhysics.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class PlayerConfig;

   class Physics : public IPhysics
   {
   public:
      Physics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
               const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
               const std::shared_ptr<PlayerConfig> playerConfig );

      void PlayerApplyFriction( const std::shared_ptr<IPlayer> player ) const override;
      void PlayerApplyGravity( const std::shared_ptr<IPlayer> player ) const override;
      void PlayerJump( const std::shared_ptr<IPlayer> player ) const override;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<PlayerConfig> _playerConfig;
   };
}
