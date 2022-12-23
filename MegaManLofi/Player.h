#pragma once

#include <memory>

#include "IPlayer.h"
#include "IPlayerInfoProvider.h"

namespace MegaManLofi
{
   class PlayerConfig;
   class IFrameActionRegistry;
   class IFrameRateProvider;

   class Player : public IPlayer,
                  public IPlayerInfoProvider
   {
   public:
      Player( const std::shared_ptr<PlayerConfig> config,
              const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
              const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      void Reset();
      void ResetPhysics();

      unsigned int GetLivesRemaining() const override { return _lives; }
      Direction GetDirection() const override { return _direction; }
      const Rectangle<long long>& GetHitBox() const override { return _hitBox; }

      void SetLivesRemaining( unsigned int lives ) override { _lives = lives; };
      void SetDirection( Direction direction ) override { _direction = direction; }

      const Coordinate<long long>& GetArenaPosition() const override { return _arenaPosition; }
      long long GetArenaPositionLeft() const override { return _arenaPosition.Left; }
      long long GetArenaPositionTop() const override { return _arenaPosition.Top; }
      void SetArenaPosition( Coordinate<long long> position ) override { _arenaPosition = position; }
      void SetArenaPositionLeft( long long left ) override { _arenaPosition.Left = left; }
      void SetArenaPositionTop( long long top ) override { _arenaPosition.Top = top; }

      long long GetVelocityX() const override { return _velocityX; }
      long long GetVelocityY() const override { return _velocityY; }
      void SetVelocityX( long long velocityX ) override { _velocityX = velocityX; }
      void SetVelocityY( long long velocityY ) override { _velocityY = velocityY; }

      bool IsMoving() const override;
      bool IsStanding() const override { return _isStanding; }
      bool IsJumping() const override { return _isJumping; }

      void SetIsStanding( bool isStanding ) override { _isStanding = isStanding; }
      void SetIsJumping( bool isJumping ) override { _isJumping = isJumping; }

      void StopX() override;
      void StopY() override;

   private:
      const std::shared_ptr<PlayerConfig> _config;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      Coordinate<long long> _arenaPosition;

      long long _velocityX;
      long long _velocityY;

      unsigned int _lives;
      Direction _direction;
      Rectangle<long long> _hitBox;

      bool _isStanding;
      bool _isJumping;
   };
}
