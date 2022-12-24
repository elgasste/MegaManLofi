#pragma once

#include <memory>

namespace MegaManLofi
{
   class IReadOnlyPlayer;

   class __declspec( novtable ) IPlayerInfoProvider
   {
   public:
      virtual const std::shared_ptr<IReadOnlyPlayer> GetPlayer() const = 0;
   };
}
