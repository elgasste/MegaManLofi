#pragma once

#include <memory>

#include "IPlayer.h"

namespace MegaManLofi
{
   class PlayerDefs;
   class IFrameActionRegistry;
   class IFrameRateProvider;

   class Player : public IPlayer
   {
   public:
      Player( const std::shared_ptr<PlayerDefs> playerDefs,
              const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
              const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      void Reset();
      void ResetPhysics();

      unsigned int GetLivesRemaining() const override { return _lives; }
      Direction GetDirection() const override { return _direction; }

      const Rectangle<float>& GetHitBox() const override { return _hitBox; }
      void SetHitBox( Rectangle<float> hitBox ) override { }

      void SetLivesRemaining( unsigned int lives ) override { _lives = lives; };
      void SetDirection( Direction direction ) override { _direction = direction; }

      int GetUniqueId() const override { return _uniqueId; }
      void SetUniqueId( int id ) override { _uniqueId = id; }

      EntityType GetEntityType() const override { return EntityType::Body; }
      void SetEntityType( EntityType type ) override { }

      int GetEntityMetaId() const override { return _metaId; }
      void SetEntityMetaId( int id ) override { _metaId = id; }

      const Coordinate<float>& GetArenaPosition() const override { return _arenaPosition; }
      float GetArenaPositionLeft() const override { return _arenaPosition.Left; }
      float GetArenaPositionTop() const override { return _arenaPosition.Top; }
      void SetArenaPosition( Coordinate<float> position ) override { _arenaPosition = position; }

      float GetVelocityX() const override { return _velocityX; }
      float GetVelocityY() const override { return _velocityY; }
      void SetVelocityX( float velocityX ) override { _velocityX = velocityX; }
      void SetVelocityY( float velocityY ) override { _velocityY = velocityY; }

      MovementType GetMovementType() const override { return _movementType; }
      void SetMovementType( MovementType type ) override { _movementType = type; }

      bool IsJumping() const override { return _isJumping; }
      void SetIsJumping( bool isJumping ) override { _isJumping = isJumping; }

      void StopX() override;
      void StopY() override;

   private:
      const std::shared_ptr<PlayerDefs> _playerDefs;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      int _uniqueId;
      int _metaId;

      Coordinate<float> _arenaPosition;

      float _velocityX;
      float _velocityY;

      unsigned int _lives;
      Direction _direction;
      Rectangle<float> _hitBox;
      MovementType _movementType;

      bool _isStanding;
      bool _isJumping;
   };
}
