#include <algorithm>

#include "PlayerPhysics.h"
#include "IFrameRateProvider.h"
#include "FrameActionRegistry.h"
#include "Player.h"
#include "FrameAction.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

PlayerPhysics::PlayerPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              const shared_ptr<FrameActionRegistry> frameActionRegistry ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _player( nullptr ),
   _lastExtendJumpFrame( 0 )
{
}

void PlayerPhysics::AssignTo( const shared_ptr<Player> player )
{
   _player = player;
   Reset();
}

void PlayerPhysics::Reset()
{
   _lastExtendJumpFrame = 0;
}

void PlayerPhysics::Tick()
{
}

void PlayerPhysics::PointTo( Direction direction ) const
{
   _player->SetDirection( direction );
}

void PlayerPhysics::PushTo( Direction direction ) const
{
   float velocityDelta = 0;

   switch ( direction )
   {
      case Direction::Left:
      case Direction::UpLeft:
      case Direction::DownLeft:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() <= -( _player->GetMaxPushVelocity() ) )
         {
            return;
         }
         velocityDelta = -( _player->GetPushAccelerationPerSecond() * _frameRateProvider->GetFrameSeconds() );
         _player->SetVelocityX( max( -( _player->GetMaxPushVelocity() ), _player->GetVelocityX() + velocityDelta ) );
         break;
      case Direction::Right:
      case Direction::UpRight:
      case Direction::DownRight:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() >= _player->GetMaxPushVelocity() )
         {
            return;
         }
         velocityDelta = _player->GetPushAccelerationPerSecond() * _frameRateProvider->GetFrameSeconds();
         _player->SetVelocityX( min( _player->GetMaxPushVelocity(), _player->GetVelocityX() + velocityDelta ) );
         break;
   }
}

void PlayerPhysics::Jump()
{
   auto movementType = _player->GetMovementType();

   if ( movementType == MovementType::Standing || movementType == MovementType::Walking )
   {
      _player->SetIsJumping( true );
      _player->SetVelocityY( -( _player->GetJumpAccelerationPerSecond() ) );
      _lastExtendJumpFrame = _frameRateProvider->GetCurrentFrame();
      _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
   }
}

void PlayerPhysics::ExtendJump()
{
   if ( !_player->IsJumping() )
   {
      return;
   }
   else if ( _player->GetVelocityY() >= 0 )
   {
      _player->SetIsJumping( false );
      return;
   }
   
   auto currentFrame = _frameRateProvider->GetCurrentFrame();

   if ( _lastExtendJumpFrame < ( currentFrame - 1 ) )
   {
      // don't allow re-extending a jump
      return;
   }
   else
   {
      _lastExtendJumpFrame = currentFrame;
      _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
   }
}
