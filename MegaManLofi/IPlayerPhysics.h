#pragma once

#include <memory>

namespace MegaManLofi
{
   class IPlayer;
   enum class Direction;

   class __declspec( novtable ) IPlayerPhysics
   {
   public:
      virtual void AssignTo( const std::shared_ptr<IPlayer> player ) = 0;
      virtual void Tick() = 0;

      virtual void PointTo( Direction direction ) const = 0;
      virtual void PushTo( Direction direction ) const = 0;
      virtual void Jump() = 0;
      virtual void ExtendJump() = 0;
   };
}
