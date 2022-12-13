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
   _frameRateProvider( frameRateProvider )
{
   Reset();
}

void Player::Reset()
{
   ResetPhysics();
   _lives = _config->DefaultLives;
}

void Player::ResetPhysics()
{
   _hitBox = _config->DefaultHitBox;
   _velocityX = _config->DefaultVelocityX;
   _velocityY = _config->DefaultVelocityY;
   _direction = _config->DefaultDirection;
   _isStanding = false;
   _isJumping = false;
}

bool Player::IsMoving() const
{
   return _velocityX != 0 || _velocityY != 0;
}

void Player::StopX()
{
   _velocityX = 0;
}

void Player::StopY()
{
   _velocityY = 0;
   _isJumping = false;
}
