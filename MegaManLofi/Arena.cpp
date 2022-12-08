#include "Arena.h"
#include "ArenaConfig.h"
#include "ArenaTile.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaConfig> config ) :
   _config( config ),
   _tiles( config->DefaultTiles ),
   _width( config->DefaultTileWidth* config->DefaultHorizontalTiles ),
   _height( config->DefaultTileHeight* config->DefaultVerticalTiles ),
   _tileWidth( config->DefaultTileWidth ),
   _tileHeight( config->DefaultTileHeight ),
   _horizontalTiles( config->DefaultHorizontalTiles ),
   _verticalTiles( config->DefaultVerticalTiles )
{
   Reset();
}

void Arena::Reset()
{
   _playerPositionX = _config->DefaultPlayerPositionX;
   _playerPositionY = _config->DefaultPlayerPositionY;
}
