#pragma once

#include "IReadOnlyPlayer.h"
#include "IEntity.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IPlayer : public IReadOnlyPlayer,
                                          public IEntity
   {
   public:
      virtual void Reset() = 0;
      virtual void ResetPhysics() = 0;

      virtual void SetLivesRemaining( unsigned int lives ) = 0;
      virtual void SetIsStanding( bool isStanding ) = 0;
      virtual void SetIsJumping( bool isJumping ) = 0;
   };
}
