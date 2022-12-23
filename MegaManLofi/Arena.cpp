#include "Arena.h"
#include "ArenaDefs.h"
#include "IPlayer.h"
#include "ArenaTile.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaDefs> arenaDefs ) :
   _arenaDefs( arenaDefs ),
   _player( nullptr ),
   _tiles( arenaDefs->DefaultTiles ),
   _width( arenaDefs->DefaultTileWidth * arenaDefs->DefaultHorizontalTiles ),
   _height( arenaDefs->DefaultTileHeight * arenaDefs->DefaultVerticalTiles ),
   _tileWidth( arenaDefs->DefaultTileWidth ),
   _tileHeight( arenaDefs->DefaultTileHeight ),
   _horizontalTiles( arenaDefs->DefaultHorizontalTiles ),
   _verticalTiles( arenaDefs->DefaultVerticalTiles )
{
   Reset();
}

void Arena::Reset()
{
   if ( _player )
   {
      _player->SetArenaPosition( _arenaDefs->DefaultPlayerPosition );
   }
}

void Arena::SetPlayer( const shared_ptr<IPlayer> player )
{
   _player = player;
}
