#include <algorithm>

#include "Physics.h"
#include "IFrameRateProvider.h"
#include "IFrameActionRegistry.h"
#include "PlayerConfig.h"
#include "IPlayer.h"
#include "FrameAction.h"

using namespace std;
using namespace MegaManLofi;

Physics::Physics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                  const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                  const shared_ptr<PlayerConfig> playerConfig ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _playerConfig( playerConfig )
{
}

void Physics::PlayerApplyFriction( const shared_ptr<IPlayer> player ) const
{
   if ( _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) )
   {
      return;
   }

   auto velocityDelta = ( _playerConfig->PushAccelerationPerSecond / _frameRateProvider->GetFramesPerSecond() );
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

   player->SetVelocityX( clamp( newVelocityX, -( _playerConfig->MaxPushVelocity ), _playerConfig->MaxPushVelocity ) );
}
