#include <algorithm>

#include "PlayerPhysics.h"
#include "IFrameRateProvider.h"
#include "FrameActionRegistry.h"
#include "PlayerPhysicsDefs.h"
#include "Player.h"
#include "FrameAction.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

PlayerPhysics::PlayerPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              const shared_ptr<FrameActionRegistry> frameActionRegistry,
                              const shared_ptr<PlayerPhysicsDefs> physicsDefs ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _physicsDefs( physicsDefs ),
   _player( nullptr ),
   _lastExtendJumpFrame( 0 )
{
}

void PlayerPhysics::AssignTo( const shared_ptr<Player> player )
{
   _player = player;
   _lastExtendJumpFrame = 0;
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
   float velocityDelta = 0;

   switch ( direction )
   {
      case Direction::Left:
      case Direction::UpLeft:
      case Direction::DownLeft:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() <= -( _physicsDefs->MaxPushVelocity ) )
         {
            return;
         }
         velocityDelta = -( _physicsDefs->PushAccelerationPerSecond * _frameRateProvider->GetFrameSeconds() );
         _player->SetVelocityX( max( -( _physicsDefs->MaxPushVelocity ), _player->GetVelocityX() + velocityDelta ) );
         break;
      case Direction::Right:
      case Direction::UpRight:
      case Direction::DownRight:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() >= _physicsDefs->MaxPushVelocity )
         {
            return;
         }
         velocityDelta = _physicsDefs->PushAccelerationPerSecond * _frameRateProvider->GetFrameSeconds();
         _player->SetVelocityX( min( _physicsDefs->MaxPushVelocity, _player->GetVelocityX() + velocityDelta ) );
         break;
   }
}

void PlayerPhysics::ApplyFriction() const
{
   if ( _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) )
   {
      return;
   }

   auto velocityDelta = _physicsDefs->FrictionDecelerationPerSecond * _frameRateProvider->GetFrameSeconds();
   auto currentVelocityX = _player->GetVelocityX();
   float newVelocityX = 0;

   if ( currentVelocityX < 0 )
   {
      newVelocityX = min( currentVelocityX + velocityDelta, 0.0f );
   }
   else if ( currentVelocityX > 0 )
   {
      newVelocityX = max( currentVelocityX - velocityDelta, 0.0f );
   }

   _player->SetVelocityX( newVelocityX );
}

void PlayerPhysics::ApplyGravity() const
{
   auto currentVelocityY = _player->GetVelocityY();
   auto gravityVelocityDelta = _physicsDefs->GravityAccelerationPerSecond * _frameRateProvider->GetFrameSeconds();

   if ( currentVelocityY < 0 )
   {
      if ( _frameActionRegistry->ActionFlagged( FrameAction::PlayerJumping ) )
      {
         _player->SetVelocityY( currentVelocityY + gravityVelocityDelta );
      }
      else
      {
         _player->SetVelocityY( 0 );
      }
   }
   else
   {
      _player->SetVelocityY( min( currentVelocityY + gravityVelocityDelta, _physicsDefs->MaxGravityVelocity ) );
   }
}

void PlayerPhysics::Jump()
{
   auto movementType = _player->GetMovementType();

   if ( movementType == MovementType::Standing || movementType == MovementType::Walking )
   {
      _player->SetIsJumping( true );
      _player->SetVelocityY( -( _physicsDefs->JumpAccelerationPerSecond ) );
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
