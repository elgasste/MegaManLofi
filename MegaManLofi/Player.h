#pragma once

#include <memory>

#include "IPlayer.h"
#include "IPlayerInfoProvider.h"
#include "Direction.h"

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
      bool IsStanding() const override { return _isStanding; }
      bool IsJumping() const override { return _isJumping; }

      void SetDirection( Direction direction ) override { _direction = direction; }

      const Rectangle& GetHitBox() const override { return _hitBox; }

      long long GetVelocityX() const override { return _velocityX; }
      long long GetVelocityY() const override { return _velocityY; }

      void SetVelocityX( long long velocityX ) override { _velocityX = velocityX; }
      void SetVelocityY( long long velocityY ) override { _velocityY = velocityY; }

      void SetIsStanding( bool isStanding ) override { _isStanding = isStanding; }
      void SetIsJumping( bool isJumping ) override { _isJumping = isJumping; }

      void StopX() override { _velocityX = 0; }
      void StopY() override { _velocityY = 0; }

   private:
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      Rectangle _hitBox;

      long long _velocityX;
      long long _velocityY;

      Direction _direction;

      bool _isStanding;
      bool _isJumping;
   };
}
