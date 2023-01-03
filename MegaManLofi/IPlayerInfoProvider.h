#pragma once

#include <memory>

namespace MegaManLofi
{
   class ReadOnlyPlayer;
   class ReadOnlyEntity;

   class __declspec( novtable ) IPlayerInfoProvider
   {
   public:
      virtual const std::shared_ptr<ReadOnlyPlayer> GetPlayer() const = 0;
      virtual const std::shared_ptr<ReadOnlyEntity> GetPlayerEntity() const = 0;
   };
}
