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

Player::Player( const shared_ptr<IFrameRateProvider> frameRateProvider,
                const shared_ptr<PlayerDefs> playerDefs,
                const shared_ptr<FrameActionRegistry> frameActionRegistry,
                const shared_ptr<GameEventAggregator> eventAggregator ) :
   Entity( frameRateProvider ),
   _playerDefs( playerDefs ),
   _frameActionRegistry( frameActionRegistry ),
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
   _damageInvulnerabilitySeconds = _playerDefs->DamageInvulnerabilitySeconds;
   _knockBackSeconds = _playerDefs->KnockBackSeconds;
   _knockBackVelocity = _playerDefs->KnockBackVelocity;

   Reset();
}

void Player::Reset()
{
   _livesRemaining = _playerDefs->DefaultLives;
   _health = _maxHealth;
   _isInvulnerable = false;
   _damageInvulnerabilityCounter = 0;
   _isKnockedBack = false;
   _knockBackCounter = 0;

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
   if ( _isKnockedBack )
   {
      return;
   }

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
   if ( _isKnockedBack )
   {
      _isJumping = false;
      return;
   }

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
   else if ( _velocityY >= 0 || _isKnockedBack )
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
   bool tookPayload = Entity::TakeCollisionPayload( payload );

   if ( tookPayload )
   {
      if ( _health == 0 )
      {
         _eventAggregator->RaiseEvent( GameEvent::CollisionDeath );
      }

      return true;
   }

   if ( payload.Lives < 0 )
   {
      _livesRemaining = (unsigned int)max( (int)_livesRemaining + payload.Lives, 0 );
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
