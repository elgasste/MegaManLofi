#pragma once

#include "IReadOnlyEntity.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IEntity : public IReadOnlyEntity
   {
   public:
      virtual void SetUniqueId( int id ) = 0;
      virtual void SetEntityType( EntityType type ) = 0;
      virtual void SetEntityMetaId( int id ) = 0;
      virtual void SetArenaPosition( Coordinate<long long> position ) = 0;
      virtual void SetVelocityX( long long velocity ) = 0;
      virtual void SetVelocityY( long long velocity ) = 0;
      virtual void SetDirection( Direction direction ) = 0;
      virtual void SetHitBox( Rectangle<long long> hitBox ) = 0;
      virtual void SetMovementType( MovementType type ) = 0;

      virtual void StopX() = 0;
      virtual void StopY() = 0;
   };
}
