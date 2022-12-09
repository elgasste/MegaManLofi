#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   enum class Direction;

   class __declspec( novtable ) IPlayerInfoProvider
   {
   public:
      virtual Direction GetDirection() const = 0;

      virtual const Rectangle& GetHitBox() const = 0;

      virtual bool IsMoving() const = 0;
      virtual bool IsStanding() const = 0;
      virtual bool IsJumping() const = 0;
   };
}
