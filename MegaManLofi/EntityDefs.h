#pragma once

#include <memory>
#include <map>

#include "EntityType.h"
#include "ItemInfo.h"
#include "ProjectileInfo.h"
#include "EnemyInfo.h"
#include "EntityCollisionPayload.h"

namespace MegaManLofi
{
   class EntityBehavior;

   class EntityDefs
   {
   public:
      std::map<int, EntityType> EntityTypeMap;
      std::map<int, ItemInfo> ItemInfoMap;
      std::map<int, ProjectileInfo> ProjectileInfoMap;
      std::map<int, EnemyInfo> EnemyInfoMap;

      std::map<int, int> EntityProjectileMap;
      std::map<int, std::shared_ptr<EntityBehavior>> EntityBehaviorMap;
      std::map<int, EntityCollisionPayload> CollisionPayloadMap;
   };
}
