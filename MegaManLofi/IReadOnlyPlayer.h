#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IReadOnlyPlayer
   {
   public:
      virtual unsigned int GetLivesRemaining() const = 0;
      virtual bool IsStanding() const = 0;
      virtual bool IsJumping() const = 0;
   };
}
