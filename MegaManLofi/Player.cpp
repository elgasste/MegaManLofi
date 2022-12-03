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

void Player::Point( Direction direction )
{
   _direction = direction;
}
