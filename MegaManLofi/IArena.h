#pragma once

#include <memory>

#include "IArenaInfoProvider.h"

namespace MegaManLofi
{
   class IPlayer;

   class __declspec( novtable ) IArena : public IArenaInfoProvider
   {
   public:
      virtual void Reset() = 0;
      virtual const std::shared_ptr<IPlayer> GetPlayer() const = 0;
      virtual void SetPlayer( const std::shared_ptr<IPlayer> player ) = 0;
   };
}
