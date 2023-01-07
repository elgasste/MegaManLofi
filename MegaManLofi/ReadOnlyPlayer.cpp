#include "ReadOnlyPlayer.h"

using namespace MegaManLofi;

ReadOnlyPlayer::ReadOnlyPlayer() :
   _livesRemaining( 0 ),
   _isJumping( false ),
   _maxPushVelocity( 0 ),
   _pushAccelerationPerSecond( 0 ),
   _jumpAccelerationPerSecond( 0 ),
   _maxJumpExtensionSeconds( 0 )
{
}
