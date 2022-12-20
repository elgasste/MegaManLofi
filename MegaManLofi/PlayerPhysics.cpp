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
                              const shared_ptr<PlayerPhysicsConfig> config ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _config( config ),
   _player( nullptr ),
   _lastExtendJumpFrame( 0 ),
   _elapsedJumpExtensionSeconds( 0. )
{
}

void PlayerPhysics::AssignTo( const shared_ptr<IPlayer> player )
{
   _player = player;

   _lastExtendJumpFrame = 0;
   _elapsedJumpExtensionSeconds = 0;
}

void PlayerPhysics::Tick()
{
   ApplyFriction();
   ApplyGravity();
}

void PlayerPhysics::PointTo( Direction direction ) const
{
   _player->SetDirection( direction );
}

void PlayerPhysics::PushTo( Direction direction ) const
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

void PlayerPhysics::Jump()
{
   if ( _player->IsStanding() )
   {
      _player->SetIsJumping( true );
      _player->SetVelocityY( -( _config->JumpAccelerationPerSecond ) );
      _lastExtendJumpFrame = _frameRateProvider->GetCurrentFrame();
      _elapsedJumpExtensionSeconds = 0.;
      _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
   }
}

void PlayerPhysics::ExtendJump()
{
   if ( !_player->IsJumping() )
   {
      return;
   }
   
   auto currentFrame = _frameRateProvider->GetCurrentFrame();

   if ( _lastExtendJumpFrame < ( currentFrame - 1 ) )
   {
      // don't allow re-extending a jump
      return;
   }
   else if ( _elapsedJumpExtensionSeconds >= _config->MaxJumpExtensionSeconds )
   {
      _player->SetIsJumping( false );
   }
   else
   {
      _lastExtendJumpFrame = currentFrame;
      _elapsedJumpExtensionSeconds += _frameRateProvider->GetFrameScalar();
      _player->SetVelocityY( -( _config->JumpAccelerationPerSecond ) );
      _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
   }
}
