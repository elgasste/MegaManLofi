#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   enum class Direction;

   class __declspec( novtable ) IPlayerInfoProvider
   {
   public:
      virtual unsigned int GetLivesRemaining() const = 0;
      virtual Direction GetDirection() const = 0;
      virtual const Rectangle<long long>& GetHitBox() const = 0;

      virtual bool IsMoving() const = 0;
      virtual bool IsStanding() const = 0;
      virtual bool IsJumping() const = 0;
   };
}
