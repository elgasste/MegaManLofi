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
   _movementType( (MovementType)0 )
{
}