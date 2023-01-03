#pragma once

#include <memory>

#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   class EntityDefs;
   class IUniqueNumberGenerator;
   class Entity;

   class EntityFactory
   {
   public:
      EntityFactory() { }
      EntityFactory( const std::shared_ptr<EntityDefs> entityDefs,
                     const std::shared_ptr<IUniqueNumberGenerator> uniqueNumberGenerator );

      virtual const std::shared_ptr<Entity> CreateBullet( Coordinate<float> position, Direction direction ) const;

   private:
      const std::shared_ptr<EntityDefs> _entityDefs;
      const std::shared_ptr<IUniqueNumberGenerator> _uniqueNumberGenerator;
   };
}
