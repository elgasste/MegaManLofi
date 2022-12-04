#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArena.h>

class mock_Arena : public MegaManLofi::IArena
{
public:
   MOCK_METHOD( double, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( double, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( double, GetPlayerPositionX, ( ), ( const, override ) );
   MOCK_METHOD( double, GetPlayerPositionY, ( ), ( const, override ) );
   MOCK_METHOD( void, SetPlayerPositionX, ( double ), ( override ) );
   MOCK_METHOD( void, SetPlayerPositionY, ( double ), ( override ) );
   MOCK_METHOD( double, GetTileWidth, ( ), ( const, override ) );
   MOCK_METHOD( double, GetTileHeight, ( ), ( const, override ) );
   MOCK_METHOD( int, GetHorizontalTiles, ( ), ( const, override ) );
   MOCK_METHOD( int, GetVerticalTiles, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ArenaTile&, GetTile, ( long long ), ( const, override ) );
};
