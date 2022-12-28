#include "EntityConsoleSpriteCopier.h"
#include "EntityConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

const shared_ptr<IEntityConsoleSprite> EntityConsoleSpriteCopier::MakeCopy( const shared_ptr<IEntityConsoleSprite> sprite ) const
{
   auto realSprite = static_pointer_cast<EntityConsoleSprite>( sprite );
   return make_shared<EntityConsoleSprite>( *realSprite );
}
