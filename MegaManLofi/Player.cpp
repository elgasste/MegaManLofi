#include <algorithm>

#include "Player.h"
#include "PlayerDefs.h"
#include "IFrameActionRegistry.h"
#include "IFrameRateProvider.h"
#include "FrameAction.h"
#include "Rectangle.h"

using namespace std;
using namespace MegaManLofi;

Player::Player( const shared_ptr<PlayerDefs> playerDefs,
                const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _playerDefs( playerDefs ),
   _frameActionRegistry( frameActionRegistry ),
   _frameRateProvider( frameRateProvider )
{
   _uniqueId = playerDefs->DefaultUniqueId;
   _entityMetaId = playerDefs->DefaultEntityMetaId;

   Reset();
}

void Player::Reset()
{
   ResetPhysics();
   _livesRemaining = _playerDefs->DefaultLives;
}

void Player::ResetPhysics()
{
   _hitBox = _playerDefs->DefaultHitBox;
   _velocityX = _playerDefs->DefaultVelocityX;
   _velocityY = _playerDefs->DefaultVelocityY;
   _direction = _playerDefs->DefaultDirection;
   _movementType = _playerDefs->DefaultMovementType;
   _isJumping = false;
}

void Player::StopY()
{
   _velocityY = 0;
   _isJumping = false;
}
