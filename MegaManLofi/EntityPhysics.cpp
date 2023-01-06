#include "EntityPhysics.h"

using namespace std;
using namespace MegaManLofi;

EntityPhysics::EntityPhysics() :
   _stage( nullptr )
{
}

void EntityPhysics::AssignTo( const shared_ptr<Stage> stage )
{
   _stage = stage;
}
