#include "BasicEntity.h"

using namespace std;
using namespace MegaManLofi;

BasicEntity::BasicEntity() :
   _arenaPosition( { 0, 0 } ),
   _velocityX( 0 ),
   _velocityY( 0 ),
   _direction( (Direction)0 ),
   _hitBox( { 0, 0, 0, 0 } )
{
}

bool BasicEntity::IsMoving() const
{
   return _velocityX != 0 || _velocityY != 0;
}
