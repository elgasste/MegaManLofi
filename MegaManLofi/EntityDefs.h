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

      std::map<int, EntityType> EntityTypeMap;
      std::map<int, ItemInfo> ItemInfoMap;
      std::map<int, ProjectileInfo> ProjectileInfoMap;
   };
}
