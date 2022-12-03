#pragma once

#include <memory>

namespace MegaManLofi
{
   class IPlayer;
   enum class Direction;

   class __declspec( novtable ) IPlayerPhysics
   {
   public:
      virtual void ApplyFriction( const std::shared_ptr<IPlayer> player ) const = 0;
      virtual void ApplyGravity( const std::shared_ptr<IPlayer> player ) const = 0;
      virtual void Push( const std::shared_ptr<IPlayer> player, Direction direction ) const = 0;
      virtual void Jump( const std::shared_ptr<IPlayer> player ) const = 0;
   };
}
