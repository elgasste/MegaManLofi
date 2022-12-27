#include "BasicEntity.h"

using namespace std;
using namespace MegaManLofi;

BasicEntity::BasicEntity() :
   _uniqueId( 0 ),
   _entityType( (EntityType)0 ),
   _metaId( 0 ),
   _arenaPosition( { 0, 0 } ),
   _velocityX( 0 ),
   _velocityY( 0 ),
   _direction( (Direction)0 ),
   _hitBox( { 0, 0, 0, 0 } ),
   _movementType( (MovementType)0 )
{
}
