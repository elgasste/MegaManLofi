#pragma once

#include "IEntity.h"

namespace MegaManLofi
{
   class BasicEntity : public IEntity
   {
   public:
      BasicEntity();

      EntityType GetEntityType() const override { return _entityType; }
      void SetEntityType( EntityType type ) { _entityType = type; }

      int GetEntityMetaId() const override { return _entityMetaId; }
      void SetEntityMetaId( int id ) { _entityMetaId = id; }

      const Coordinate<long long>& GetArenaPosition() const override { return _arenaPosition; }
      long long GetArenaPositionLeft() const override { return _arenaPosition.Left; }
      long long GetArenaPositionTop() const override { return _arenaPosition.Top; }
      void SetArenaPosition( Coordinate<long long> position ) { _arenaPosition = position; }

      long long GetVelocityX() const override { return _velocityX; }
      void SetVelocityX( long long velocity ) { _velocityX = velocity; }

      long long GetVelocityY() const override { return _velocityY; }
      void SetVelocityY( long long velocity ) { _velocityY = velocity; }

      Direction GetDirection() const override { return _direction; }
      void SetDirection( Direction direction ) { _direction = direction; }

      const Rectangle<long long>& GetHitBox() const override { return _hitBox; }
      void SetHitBox( Rectangle<long long> hitBox ) { _hitBox = hitBox; }

      bool IsMoving() const override;

   private:
      int _entityMetaId;
      EntityType _entityType;
      Coordinate<long long> _arenaPosition;
      long long _velocityX;
      long long _velocityY;
      Direction _direction;
      Rectangle<long long> _hitBox;
   };
}
