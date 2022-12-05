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
   _frameActionRegistry( frameActionRegistry ),
   _frameRateProvider( frameRateProvider ),
   _hitBox( config->DefaultHitBox ),
   _velocityX( config->DefaultVelocityX ),
   _velocityY( config->DefaultVelocityY ),
   _direction( config->DefaultDirection )
{
}

bool Player::IsMoving() const
{
   return _velocityX != 0 || _velocityY != 0;
}
