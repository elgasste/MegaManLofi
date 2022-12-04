#pragma once

#include <memory>

#include "IPlayer.h"
#include "IPlayerInfoProvider.h"

namespace MegaManLofi
{
   class PlayerConfig;
   class IFrameActionRegistry;
   class IFrameRateProvider;

   class Player : public IPlayer
   {
   public:
      Player( const std::shared_ptr<PlayerConfig> config,
              const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
              const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      Direction GetDirection() const override { return _direction; }
      bool IsMoving() const override;

      const Rectangle& GetHitBox() const override { return _hitBox; }

      double GetVelocityX() const override { return _velocityX; }
      double GetVelocityY() const override { return _velocityY; }

      void SetVelocityX( double velocityX ) override { _velocityX = velocityX; }
      void SetVelocityY( double velocityY ) override { _velocityY = velocityY; }

      void Point( Direction direction ) override;
      void StopX() override { _velocityX = 0; }
      void StopY() override { _velocityY = 0; }

   private:
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      Rectangle _hitBox;

      double _velocityX;
      double _velocityY;

      Direction _direction;
   };
}
