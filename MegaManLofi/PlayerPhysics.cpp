#include <algorithm>

#include "PlayerPhysics.h"
#include "IFrameRateProvider.h"
#include "IFrameActionRegistry.h"
#include "PlayerPhysicsConfig.h"
#include "IPlayer.h"
#include "FrameAction.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

PlayerPhysics::PlayerPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                              const shared_ptr<IPlayer> player,
                              const shared_ptr<PlayerPhysicsConfig> config ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _player( player ),
   _config( config )
{
}

void PlayerPhysics::ApplyFriction() const
{
   if ( _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) )
   {
      return;
   }

   auto velocityDelta = ( _config->FrictionDecelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
   auto currentVelocityX = _player->GetVelocityX();
   auto newVelocityX = 0ll;

   if ( currentVelocityX < 0 )
   {
      newVelocityX = min( currentVelocityX + velocityDelta, 0ll );
   }
   else if ( currentVelocityX > 0 )
   {
      newVelocityX = max( currentVelocityX - velocityDelta, 0ll );
   }

   _player->SetVelocityX( newVelocityX );
}

void PlayerPhysics::ApplyGravity() const
{
   if ( !_frameActionRegistry->ActionFlagged( FrameAction::PlayerJumping ) &&
        _player->GetVelocityY() < _config->MaxGravityVelocity )
   {
      auto velocityDelta = ( _config->GravityAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
      _player->SetVelocityY( min( _player->GetVelocityY() + velocityDelta, _config->MaxGravityVelocity ) );
   }
}

void PlayerPhysics::Point( Direction direction ) const
{
   _player->SetDirection( direction );
}

void PlayerPhysics::Push( Direction direction ) const
{
   auto velocityDelta = 0ll;

   switch ( direction )
   {
      case Direction::Left:
      case Direction::UpLeft:
      case Direction::DownLeft:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() <= -( _config->MaxPushVelocity ) )
         {
            return;
         }
         velocityDelta = -( _config->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
         _player->SetVelocityX( max( -( _config->MaxPushVelocity ), _player->GetVelocityX() + velocityDelta ) );
         break;
      case Direction::Right:
      case Direction::UpRight:
      case Direction::DownRight:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() >= _config->MaxPushVelocity )
         {
            return;
         }
         velocityDelta = _config->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond();
         _player->SetVelocityX( min( _config->MaxPushVelocity, _player->GetVelocityX() + velocityDelta ) );
         break;
   }
}

void PlayerPhysics::Jump() const
{
   if ( _player->IsStanding() )
   {
      _player->SetVelocityY( -( _config->JumpAccelerationPerSecond ) );
      _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
   }
}

void PlayerPhysics::ExtendJump() const
{
   // MUFFINS: only allow this if we haven't reached the maximum extension time
}
