#pragma once

#include <memory>

#include "Direction.h"

namespace MegaManLofi
{
   class Player;

   class __declspec( novtable ) IPlayerPhysics
   {
   public:
      virtual void AssignTo( const std::shared_ptr<Player> player ) = 0;
      virtual void Tick() = 0;

      virtual void PointTo( Direction direction ) const = 0;
      virtual void PushTo( Direction direction ) const = 0;
      virtual void Jump() = 0;
      virtual void ExtendJump() = 0;
   };
}
