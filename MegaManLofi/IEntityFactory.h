#pragma once

#include <memory>

#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   class IEntity;

   class __declspec( novtable ) IEntityFactory
   {
   public:
      virtual const std::shared_ptr<IEntity> CreateBullet( Coordinate<float> position, Direction direction ) const = 0;
   };
}
