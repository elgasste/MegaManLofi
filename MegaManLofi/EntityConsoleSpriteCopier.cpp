#include "EntityConsoleSpriteCopier.h"
#include "EntityConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

const shared_ptr<EntityConsoleSprite> EntityConsoleSpriteCopier::MakeCopy( const shared_ptr<EntityConsoleSprite> sprite ) const
{
   return make_shared<EntityConsoleSprite>( *sprite );
}
