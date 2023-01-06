#pragma once

#include <map>

#include "EntityType.h"
#include "ItemInfo.h"
#include "ProjectileInfo.h"

namespace MegaManLofi
{
   class EntityDefs
   {
   public:
      int BulletEntityMetaId = 0;
      float BulletVelocity = 0;
      Rectangle<float> BulletHitBox;

      std::map<int, EntityType> EntityTypeMap;
      std::map<int, ItemInfo> ItemInfoMap;
      std::map<int, ProjectileInfo> ProjectileInfoMap;
   };
}
