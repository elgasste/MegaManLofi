#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IReadOnlyArena.h>

class mock_ReadOnlyArena : public MegaManLofi::IReadOnlyArena
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IReadOnlyPlayer>, GetPlayer, ( ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IReadOnlyEntity>, GetEntity, ( int ), ( const, override ) );
   MOCK_METHOD( int, GetEntityCount, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Quad<float>&, GetActiveRegion, ( ), ( const, override ) );
   MOCK_METHOD( float, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( float, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( float, GetTileWidth, ( ), ( const, override ) );
   MOCK_METHOD( float, GetTileHeight, ( ), ( const, override ) );
   MOCK_METHOD( int, GetHorizontalTiles, ( ), ( const, override ) );
   MOCK_METHOD( int, GetVerticalTiles, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ArenaTile&, GetTile, ( int ), ( const, override ) );
   MOCK_METHOD( bool, HasEntity, ( int ), ( const, override ) );
};
