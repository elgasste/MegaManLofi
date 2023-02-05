#include "Entity.h"
#include "IBehavior.h"

using namespace std;
using namespace MegaManLofi;

Entity::Entity() :
   _behavior( nullptr )
{
}

void Entity::SetBehavior( const shared_ptr<IBehavior> behavior )
{
   _behavior = behavior;
}

void Entity::Act()
{
   if ( _behavior )
   {
      _behavior->Tick();
   }
}
