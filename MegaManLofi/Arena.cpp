#include "Arena.h"
#include "ArenaConfig.h"
#include "IPlayer.h"
#include "ArenaTile.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaConfig> config ) :
   _config( config ),
   _player( nullptr ),
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
   if ( _player )
   {
      _player->SetArenaPosition( _config->DefaultPlayerPosition );
   }
}

void Arena::SetPlayer( const shared_ptr<IPlayer> player )
{
   _player = player;
}
