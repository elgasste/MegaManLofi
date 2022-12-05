#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArena.h>

class mock_Arena : public MegaManLofi::IArena
{
public:
   MOCK_METHOD( long long, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetPlayerPositionX, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetPlayerPositionY, ( ), ( const, override ) );
   MOCK_METHOD( void, SetPlayerPositionX, ( long long ), ( override ) );
   MOCK_METHOD( void, SetPlayerPositionY, ( long long ), ( override ) );
   MOCK_METHOD( long long, GetTileWidth, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetTileHeight, ( ), ( const, override ) );
   MOCK_METHOD( int, GetHorizontalTiles, ( ), ( const, override ) );
   MOCK_METHOD( int, GetVerticalTiles, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ArenaTile&, GetTile, ( long long ), ( const, override ) );
};
