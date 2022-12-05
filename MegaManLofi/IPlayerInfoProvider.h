#pragma once

namespace MegaManLofi
{
   enum class Direction;

   class __declspec( novtable ) IPlayerInfoProvider
   {
   public:
      virtual Direction GetDirection() const = 0;
      virtual bool IsMoving() const = 0;
      virtual bool IsStanding() const = 0;
   };
}
