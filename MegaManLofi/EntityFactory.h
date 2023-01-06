#pragma once

#include <memory>

#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   class EntityDefs;
   class UniqueNumberGenerator;
   class Entity;

   class EntityFactory
   {
   public:
      EntityFactory() { }
      EntityFactory( const std::shared_ptr<EntityDefs> entityDefs,
                     const std::shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator );

      virtual const std::shared_ptr<Entity> CreateBullet( Coordinate<float> position, Direction direction ) const;
      virtual const std::shared_ptr<Entity> CreateEntity( int entityMetaId, Direction direction ) const;

   private:
      void SetProjectileInfo( const std::shared_ptr<Entity> projectile, Direction direction ) const;

   private:
      const std::shared_ptr<EntityDefs> _entityDefs;
      const std::shared_ptr<UniqueNumberGenerator> _uniqueNumberGenerator;
   };
}
