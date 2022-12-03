#include <algorithm>

#include "PlayerPhysics.h"
#include "IFrameRateProvider.h"
#include "IFrameActionRegistry.h"
#include "PlayerConfig.h"
#include "IPlayer.h"
#include "FrameAction.h"

using namespace std;
using namespace MegaManLofi;

PlayerPhysics::PlayerPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                              const shared_ptr<PlayerConfig> playerConfig ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _playerConfig( playerConfig )
{
}

void PlayerPhysics::ApplyFriction( const shared_ptr<IPlayer> player ) const
{
   if ( _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) )
   {
      return;
   }

   auto velocityDelta = ( _playerConfig->FrictionDecelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
   auto currentVelocityX = player->GetVelocityX();
   auto newVelocityX = 0.;

   if ( currentVelocityX < 0. )
   {
      newVelocityX = min( currentVelocityX + velocityDelta, 0. );
   }
   else if ( currentVelocityX > 0. )
   {
      newVelocityX = max( currentVelocityX - velocityDelta, 0. );
   }

   player->SetVelocityX( newVelocityX );
}

void PlayerPhysics::ApplyGravity( const shared_ptr<IPlayer> player ) const
{
   if ( !_frameActionRegistry->ActionFlagged( FrameAction::PlayerJumping ) &&
        player->GetVelocityY() < _playerConfig->MaxGravityVelocity )
   {
      auto velocityDelta = ( _playerConfig->GravityAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
      player->SetVelocityY( min( player->GetVelocityY() + velocityDelta, _playerConfig->MaxGravityVelocity ) );
   }
}

void PlayerPhysics::Push( const shared_ptr<IPlayer> player, Direction direction ) const
{
   auto velocityDelta = 0.;

   switch ( direction )
   {
      case Direction::Left:
      case Direction::UpLeft:
      case Direction::DownLeft:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( player->GetVelocityX() <= -( _playerConfig->MaxPushVelocity ) )
         {
            return;
         }
         velocityDelta = -( _playerConfig->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
         player->SetVelocityX( max( -( _playerConfig->MaxPushVelocity ), player->GetVelocityX() + velocityDelta ) );
         break;
      case Direction::Right:
      case Direction::UpRight:
      case Direction::DownRight:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( player->GetVelocityX() >= _playerConfig->MaxPushVelocity )
         {
            return;
         }
         velocityDelta = _playerConfig->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond();
         player->SetVelocityX( min( _playerConfig->MaxPushVelocity, player->GetVelocityX() + velocityDelta ) );
         break;
   }
}

void PlayerPhysics::Jump( const shared_ptr<IPlayer> player ) const
{
   // TODO: this should only be possible if we're on a flat surface.
   player->SetVelocityY( -( _playerConfig->MaxGravityVelocity ) );
   _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
}
