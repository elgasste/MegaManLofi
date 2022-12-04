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
                              const shared_ptr<IPlayer> player,
                              const shared_ptr<PlayerConfig> playerConfig ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _player( player ),
   _playerConfig( playerConfig )
{
}

void PlayerPhysics::ApplyFriction() const
{
   if ( _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) )
   {
      return;
   }

   auto velocityDelta = ( _playerConfig->FrictionDecelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
   auto currentVelocityX = _player->GetVelocityX();
   auto newVelocityX = 0.;

   if ( currentVelocityX < 0. )
   {
      newVelocityX = min( currentVelocityX + velocityDelta, 0. );
   }
   else if ( currentVelocityX > 0. )
   {
      newVelocityX = max( currentVelocityX - velocityDelta, 0. );
   }

   _player->SetVelocityX( newVelocityX );
}

void PlayerPhysics::ApplyGravity() const
{
   if ( !_frameActionRegistry->ActionFlagged( FrameAction::PlayerJumping ) &&
        _player->GetVelocityY() < _playerConfig->MaxGravityVelocity )
   {
      auto velocityDelta = ( _playerConfig->GravityAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
      _player->SetVelocityY( min( _player->GetVelocityY() + velocityDelta, _playerConfig->MaxGravityVelocity ) );
   }
}

void PlayerPhysics::Push( Direction direction ) const
{
   auto velocityDelta = 0.;

   switch ( direction )
   {
      case Direction::Left:
      case Direction::UpLeft:
      case Direction::DownLeft:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() <= -( _playerConfig->MaxPushVelocity ) )
         {
            return;
         }
         velocityDelta = -( _playerConfig->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
         _player->SetVelocityX( max( -( _playerConfig->MaxPushVelocity ), _player->GetVelocityX() + velocityDelta ) );
         break;
      case Direction::Right:
      case Direction::UpRight:
      case Direction::DownRight:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _player->GetVelocityX() >= _playerConfig->MaxPushVelocity )
         {
            return;
         }
         velocityDelta = _playerConfig->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond();
         _player->SetVelocityX( min( _playerConfig->MaxPushVelocity, _player->GetVelocityX() + velocityDelta ) );
         break;
   }
}

void PlayerPhysics::Jump() const
{
   // TODO: this should only be possible if we're on a flat surface.
   _player->SetVelocityY( -( _playerConfig->MaxGravityVelocity ) );
   _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
}
