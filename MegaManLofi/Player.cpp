#include <algorithm>

#include "Player.h"
#include "PlayerDefs.h"
#include "FrameActionRegistry.h"
#include "IFrameRateProvider.h"
#include "GameEventAggregator.h"
#include "FrameAction.h"
#include "Rectangle.h"

using namespace std;
using namespace MegaManLofi;

Player::Player( const shared_ptr<PlayerDefs> playerDefs,
                const shared_ptr<FrameActionRegistry> frameActionRegistry,
                const shared_ptr<IFrameRateProvider> frameRateProvider,
                const shared_ptr<GameEventAggregator> eventAggregator ) :
   _playerDefs( playerDefs ),
   _frameActionRegistry( frameActionRegistry ),
   _frameRateProvider( frameRateProvider ),
   _eventAggregator( eventAggregator )
{
   _uniqueId = playerDefs->DefaultUniqueId;
   _entityMetaId = playerDefs->DefaultEntityMetaId;
   _maxGravityVelocity = _playerDefs->MaxGravityVelocity;
   _gravityAccelerationPerSecond = _playerDefs->GravityAccelerationPerSecond;
   _frictionDecelerationPerSecond = _playerDefs->FrictionDecelerationPerSecond;
   _maxPushVelocity = _playerDefs->MaxPushVelocity;
   _pushAccelerationPerSecond = _playerDefs->PushAccelerationPerSecond;
   _jumpAccelerationPerSecond = _playerDefs->JumpAccelerationPerSecond;
   _maxJumpExtensionSeconds = _playerDefs->MaxJumpExtensionSeconds;
   _maxHealth = _playerDefs->MaxHealth;

   Reset();
}

void Player::Reset()
{
   _livesRemaining = _playerDefs->DefaultLives;
   _health = _maxHealth;
   ResetPosition();
}

void Player::ResetPosition()
{
   _hitBox = _playerDefs->DefaultHitBox;
   _velocityX = _playerDefs->DefaultVelocityX;
   _velocityY = _playerDefs->DefaultVelocityY;
   _direction = _playerDefs->DefaultDirection;
   _movementType = _playerDefs->DefaultMovementType;
   _isJumping = false;
   _lastExtendJumpFrame = 0;
}

void Player::ResetHealth()
{
   _health = _playerDefs->MaxHealth;
}

void Player::PushTo( Direction direction )
{
   float velocityDelta = 0;

   switch ( direction )
   {
      case Direction::Left:
      case Direction::UpLeft:
      case Direction::DownLeft:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _velocityX <= -_maxPushVelocity )
         {
            return;
         }
         velocityDelta = -( _pushAccelerationPerSecond * _frameRateProvider->GetFrameSeconds() );
         _velocityX = max( -_maxPushVelocity, _velocityX + velocityDelta );
         break;
      case Direction::Right:
      case Direction::UpRight:
      case Direction::DownRight:
         _frameActionRegistry->FlagAction( FrameAction::PlayerPushed );
         if ( _velocityX >= _maxPushVelocity )
         {
            return;
         }
         velocityDelta = _pushAccelerationPerSecond * _frameRateProvider->GetFrameSeconds();
         _velocityX = min( _maxPushVelocity, _velocityX + velocityDelta );
         break;
   }
}

void Player::Jump()
{
   if ( _movementType == MovementType::Standing || _movementType == MovementType::Walking )
   {
      _isJumping = true;
      _velocityY = -_jumpAccelerationPerSecond;
      _lastExtendJumpFrame = _frameRateProvider->GetCurrentFrame();
      _frameActionRegistry->FlagAction( FrameAction::PlayerJumping );
   }
}

void Player::ExtendJump()
{
   if ( !_isJumping )
   {
      return;
   }
   else if ( _velocityY >= 0 )
   {
      _isJumping = false;
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

void Player::StopY()
{
   _velocityY = 0;
   _isJumping = false;
}

bool Player::TakeCollisionPayload( const EntityCollisionPayload& payload )
{
   bool tookPayload = false;

   if ( payload.Health < 0 )
   {
      _health = ( (unsigned int)abs( payload.Health ) >= _health ) ? 0 : _health + payload.Health;
      tookPayload = true;

      if ( _health == 0 )
      {
         _eventAggregator->RaiseEvent( GameEvent::CollisionDeath );
      }
   }
   else if ( payload.Health > 0 && _health < _maxHealth )
   {
      _health = min( _health + payload.Health, _maxHealth );
      tookPayload = true;
   }

   if ( payload.Lives < 0 )
   {
      _livesRemaining = ( (unsigned int)abs( payload.Lives ) >= _livesRemaining ) ? 0 : _livesRemaining + payload.Lives;
      tookPayload = true;

      if ( _livesRemaining == 0 )
      {
         _eventAggregator->RaiseEvent( GameEvent::CollisionDeath );
      }
   }
   else if ( payload.Lives > 0 )
   {
      _livesRemaining += payload.Lives;
      tookPayload = true;
   }

   return tookPayload;
}
