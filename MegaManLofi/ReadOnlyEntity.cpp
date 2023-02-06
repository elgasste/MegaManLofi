#include "ReadOnlyEntity.h"

using namespace MegaManLofi;

ReadOnlyEntity::ReadOnlyEntity() :
   _uniqueId( 0 ),
   _entityType( (EntityType)0 ),
   _entityMetaId( 0 ),
   _arenaPosition( { 0, 0 } ),
   _velocityX( 0 ),
   _velocityY( 0 ),
   _direction( (Direction)0 ),
   _hitBox( { 0, 0, 0, 0 } ),
   _movementType( (MovementType)0 ),
   _maxGravityVelocity( 0 ),
   _gravityAccelerationPerSecond( 0 ),
   _frictionDecelerationPerSecond( 0 ),
   _health( 0 ),
   _maxHealth( 0 ),
   _isInvulnerable( false )
{
}
