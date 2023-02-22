#include "Entity.h"
#include "IFrameRateProvider.h"
#include "IBehavior.h"

using namespace std;
using namespace MegaManLofi;

Entity::Entity( const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _frameRateProvider( frameRateProvider ),
   _behavior( nullptr )
{
}

void Entity::SetBehavior( const shared_ptr<IBehavior> behavior )
{
   _behavior = behavior;
}

void Entity::Tick()
{
   if ( _isInvulnerable )
   {
      _damageInvulnerabilityCounter += _frameRateProvider->GetFrameSeconds();

      if ( _damageInvulnerabilityCounter > _damageInvulnerabilitySeconds )
      {
         _isInvulnerable = false;
         _damageInvulnerabilityCounter = 0;
      }
   }

   if ( _isKnockedBack )
   {
      _knockBackCounter += _frameRateProvider->GetFrameSeconds();

      if ( _knockBackCounter > _knockBackSeconds )
      {
         _isKnockedBack = false;
         _damageInvulnerabilityCounter = 0;
      }
   }

   if ( _behavior )
   {
      _behavior->Tick();
   }
}

bool Entity::TakeCollisionPayload( const EntityCollisionPayload& payload )
{
   // only deal with health changes in here
   if ( payload.Health == 0 || ( payload.Health < 0 && _isInvulnerable ) )
   {
      return false;
   }

   auto newHealth = (unsigned int)min( max( (int)_health + payload.Health, 0 ), (int)_maxHealth );

   if ( newHealth == _health )
   {
      return false;
   }

   SetHealth( newHealth );

   if ( payload.Health < 0 && _health > 0 )
   {
      if ( _damageInvulnerabilitySeconds > 0 )
      {
         _isInvulnerable = true;
         _damageInvulnerabilityCounter = 0;
      }

      CheckKnockBack();
   }

   return true;
}

void Entity::CheckKnockBack()
{
   if ( _isKnockedBack || _knockBackSeconds <= 0 )
   {
      return;
   }

   _isKnockedBack = true;
   _knockBackCounter = 0;
   _velocityY = 0;

   // TODO: we can get fancier with this, like checking which direction the
   // attack came from, and knock back in that direction. For now let's keep
   // it simple and just knock back from the direction we're facing.
   // (if we're facing straight up or down, pretend we're facing right)
   switch ( _direction )
   {
      case Direction::UpLeft:
      case Direction::Left:
      case Direction::DownLeft:
         _velocityX = _knockBackVelocity;
         break;
      default:
         _velocityX = -_knockBackVelocity;
         break;
   }
}
