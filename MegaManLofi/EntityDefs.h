#pragma once

#include <memory>
#include <map>

#include "EntityType.h"
#include "ItemInfo.h"
#include "ProjectileInfo.h"
#include "EntityCollisionPayload.h"

namespace MegaManLofi
{
   class IBehavior;

   class EntityDefs
   {
   public:
      int BulletEntityMetaId = 0;

      std::map<int, EntityType> EntityTypeMap;
      std::map<int, ItemInfo> ItemInfoMap;
      std::map<int, ProjectileInfo> ProjectileInfoMap;

      std::map<int, std::shared_ptr<IBehavior>> EntityBehaviorMap;
      std::map<int, EntityCollisionPayload> CollisionPayloadMap;
   };
}
