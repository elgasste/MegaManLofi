#pragma once

#include <map>
#include <vector>

#include "EntityType.h"
#include "ItemInfo.h"
#include "ProjectileInfo.h"
#include "EnemyInfo.h"
#include "EntityCollisionPayload.h"
#include "MbcDefines.h"

namespace MegaManLofi
{
   class EntityDefs
   {
   public:
      std::map<int, EntityType> EntityTypeMap;
      std::map<int, ItemInfo> ItemInfoMap;
      std::map<int, ProjectileInfo> ProjectileInfoMap;
      std::map<int, EnemyInfo> EnemyInfoMap;

      std::map<int, int> EntityProjectileMap;
      std::map<int, std::vector<mbc_instruction>> EntityBehaviorMap;
      std::map<int, EntityCollisionPayload> CollisionPayloadMap;
   };
}
