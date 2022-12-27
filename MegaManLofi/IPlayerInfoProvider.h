#pragma once

#include <memory>

namespace MegaManLofi
{
   class IReadOnlyPlayer;
   class IReadOnlyEntity;

   class __declspec( novtable ) IPlayerInfoProvider
   {
   public:
      virtual const std::shared_ptr<IReadOnlyPlayer> GetPlayer() const = 0;
      virtual const std::shared_ptr<IReadOnlyEntity> GetPlayerEntity() const = 0;
   };
}
