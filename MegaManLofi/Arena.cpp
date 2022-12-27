#include <algorithm>

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
   _entities.clear();

   if ( _player )
   {
      _player->SetArenaPosition( _arenaDefs->DefaultPlayerPosition );
      AddEntity( _player );
   }
}

const shared_ptr<IReadOnlyPlayer> Arena::GetPlayer() const
{
   return _player;
}

void Arena::SetPlayer( const shared_ptr<IPlayer> player )
{
   _player = player;
   AddEntity( player );
}

const shared_ptr<IReadOnlyEntity> Arena::GetEntity( int index ) const
{
   return _entities[index];
}

void Arena::AddEntity( const std::shared_ptr<IEntity> entity )
{
   if ( find( _entities.begin(), _entities.end(), entity ) == _entities.end() )
   {
      _entities.push_back( entity );
   }
}

void Arena::RemoveEntity( const std::shared_ptr<IEntity> entity )
{
   for ( auto it = _entities.begin(); it != _entities.end(); it++ )
   {
      if ( *it == entity )
      {
         _entities.erase( it );
         break;
      }
   }
}
