#pragma once

namespace MegaManLofi
{
   enum class Direction;

   class __declspec( novtable ) IPlayerPhysics
   {
   public:
      virtual void ApplyFriction() const = 0;
      virtual void ApplyGravity() const = 0;
      virtual void Point( Direction direction ) const = 0;
      virtual void Push( Direction direction ) const = 0;
      virtual void Jump() const = 0;
   };
}
