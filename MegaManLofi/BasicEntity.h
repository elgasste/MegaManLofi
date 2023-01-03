#pragma once

#include "IEntity.h"

namespace MegaManLofi
{
   class BasicEntity : public IEntity
   {
   public:
      BasicEntity();

      int GetUniqueId() const override { return _uniqueId; }
      void SetUniqueId( int id ) override { _uniqueId = id; }

      EntityType GetEntityType() const override { return _entityType; }
      void SetEntityType( EntityType type ) { _entityType = type; }

      int GetEntityMetaId() const override { return _metaId; }
      void SetEntityMetaId( int id ) { _metaId = id; }

      const Coordinate<float>& GetArenaPosition() const override { return _arenaPosition; }
      float GetArenaPositionLeft() const override { return _arenaPosition.Left; }
      float GetArenaPositionTop() const override { return _arenaPosition.Top; }
      void SetArenaPosition( Coordinate<float> position ) { _arenaPosition = position; }

      float GetVelocityX() const override { return _velocityX; }
      void SetVelocityX( float velocity ) { _velocityX = velocity; }

      float GetVelocityY() const override { return _velocityY; }
      void SetVelocityY( float velocity ) { _velocityY = velocity; }

      Direction GetDirection() const override { return _direction; }
      void SetDirection( Direction direction ) { _direction = direction; }

      const Rectangle<float>& GetHitBox() const override { return _hitBox; }
      void SetHitBox( Rectangle<float> hitBox ) { _hitBox = hitBox; }

      MovementType GetMovementType() const override { return _movementType; }
      void SetMovementType( MovementType type ) override { _movementType = type; }

      void StopX() override { _velocityX = 0; }
      void StopY() override { _velocityY = 0; }

   private:
      int _uniqueId;
      EntityType _entityType;
      int _metaId;
      Coordinate<float> _arenaPosition;
      float _velocityX;
      float _velocityY;
      Direction _direction;
      Rectangle<float> _hitBox;
      MovementType _movementType;
   };
}
