#pragma once

#include "IEntity.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IReadOnlyPlayer : public IEntity
   {
   public:
      virtual unsigned int GetLivesRemaining() const = 0;
      virtual bool IsStanding() const = 0;
      virtual bool IsJumping() const = 0;
   };
}
