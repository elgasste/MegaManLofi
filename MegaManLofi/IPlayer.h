#pragma once

#include "IReadOnlyPlayer.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IPlayer : public IReadOnlyPlayer
   {
   public:
      virtual void Reset() = 0;
      virtual void ResetPhysics() = 0;

      virtual void SetLivesRemaining( unsigned int lives ) = 0;
      virtual void SetDirection( Direction direction ) = 0;
      virtual void SetArenaPosition( Coordinate<long long> position ) = 0;
      virtual void SetArenaPositionLeft( long long left ) = 0;
      virtual void SetArenaPositionTop( long long top ) = 0;
      virtual void SetVelocityX( long long velocityX ) = 0;
      virtual void SetVelocityY( long long velocityY ) = 0;
      virtual void SetIsStanding( bool isStanding ) = 0;
      virtual void SetIsJumping( bool isJumping ) = 0;

      virtual void StopX() = 0;
      virtual void StopY() = 0;
   };
}
