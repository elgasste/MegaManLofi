#pragma once

#include <memory>

namespace MegaManLofi
{
   class IPlayer;

   class __declspec( novtable ) IPhysics
   {
   public:
      virtual void PlayerApplyFriction( const std::shared_ptr<IPlayer> player ) const = 0;
   };
}
