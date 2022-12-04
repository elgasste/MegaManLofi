#include "Arena.h"
#include "ArenaConfig.h"
#include "ArenaTile.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaConfig> config ) :
   _tiles( config->Tiles ),
   _width( config->TileWidth* config->HorizontalTiles ),
   _height( config->TileHeight* config->VerticalTiles ),
   _playerPositionX( config->PlayerStartX ),
   _playerPositionY( config->PlayerStartY )
{
}
