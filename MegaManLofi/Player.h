#pragma once

#include <memory>

#include "IPlayer.h"
#include "IPlayerInfoProvider.h"
#include "Rectangle.h"

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

      void Push( Direction direction ) override;
      void Point( Direction direction ) override;
      void Jump() override;
      void ApplyFriction() override;
      void ApplyGravity() override;
      void StopX() override;
      void StopY() override;

   private:
      void ClampVelocity();

   private:
      const std::shared_ptr<PlayerConfig> _config;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      Rectangle _hitBox;

      double _velocityX;
      double _velocityY;

      Direction _direction;
   };
}
