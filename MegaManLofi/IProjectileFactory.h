#pragma once

#include <memory>

#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   class IEntity;

   class __declspec( novtable ) IProjectileFactory
   {
   public:
      virtual const std::shared_ptr<IEntity> CreateBullet( Coordinate<long long> position, Direction direction ) const = 0;
   };
}
