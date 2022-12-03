#include <algorithm>

#include "Player.h"
#include "PlayerConfig.h"
#include "IFrameActionRegistry.h"
#include "IFrameRateProvider.h"
#include "FrameAction.h"
#include "Rectangle.h"

using namespace std;
using namespace MegaManLofi;

Player::Player( const shared_ptr<PlayerConfig> config,
                const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _config( config ),
   _frameActionRegistry( frameActionRegistry ),
   _frameRateProvider( frameRateProvider ),
   _hitBox( config->HitBox ),
   _velocityX( _config->StartVelocityX ),
   _velocityY( _config->StartVelocityY ),
   _direction( _config->StartDirection )
{
}

bool Player::IsMoving() const
{
   return _velocityX != 0. || _velocityY != 0.;
}

void Player::Push( Direction direction )
{
   switch ( direction )
   {
   case Direction::Left:
   case Direction::UpLeft:
   case Direction::DownLeft:
      _velocityX -= ( _config->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
      _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
      break;
   case Direction::Right:
   case Direction::UpRight:
   case Direction::DownRight:
      _velocityX += ( _config->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
      _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
      break;
   }

   ClampVelocity();
}

void Player::Point( Direction direction )
{
   _direction = direction;
}

void Player::Jump()
{
   // TODO: this should only be possible if we're on a flat surface.
   _velocityY = -( _config->MaxGravityVelocity );
   _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
}

void Player::StopX()
{
   _velocityX = 0.;
}

void Player::StopY()
{
   _velocityY = 0.;
}

void Player::ClampVelocity()
{
   if ( _velocityX < -( _config->MaxPushVelocity ) )
   {
      _velocityX = -( _config->MaxPushVelocity );
   }
   else if ( _velocityX > _config->MaxPushVelocity )
   {
      _velocityX = _config->MaxPushVelocity;
   }

   if ( _velocityY < -( _config->MaxGravityVelocity ) )
   {
      _velocityY = -( _config->MaxGravityVelocity );
   }
   else if ( _velocityY > _config->MaxGravityVelocity )
   {
      _velocityY = _config->MaxGravityVelocity;
   }
}
