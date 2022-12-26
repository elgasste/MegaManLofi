#pragma once

#include "IEntityFactory.h"

namespace MegaManLofi
{
   class EntityDefs;

   class EntityFactory : public IEntityFactory
   {
   public:
      EntityFactory( const std::shared_ptr<EntityDefs> entityDefs );

      const std::shared_ptr<IEntity> CreateBullet( Coordinate<long long> position, Direction direction ) const override;

   private:
      const std::shared_ptr<EntityDefs> _entityDefs;
   };
}