#pragma once

#include "IPlayerInfoProvider.h"

namespace MegaManLofi
{
   enum class Direction;

   class __declspec( novtable ) IPlayer : public IPlayerInfoProvider
   {
   public:
      virtual void Reset() = 0;

      virtual void SetLivesRemaining( unsigned int lives ) = 0;
      virtual void SetDirection( Direction direction ) = 0;

      virtual long long GetVelocityX() const = 0;
      virtual long long GetVelocityY() const = 0;

      virtual void SetVelocityX( long long velocityX ) = 0;
      virtual void SetVelocityY( long long velocityY ) = 0;

      virtual void SetIsStanding( bool isStanding ) = 0;
      virtual void SetIsJumping( bool isJumping ) = 0;

      virtual void StopX() = 0;
      virtual void StopY() = 0;
   };
}
