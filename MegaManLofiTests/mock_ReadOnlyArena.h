#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IReadOnlyArena.h>

class mock_ReadOnlyArena : public MegaManLofi::IReadOnlyArena
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IReadOnlyPlayer>, GetPlayer, ( ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IReadOnlyEntity>, GetEntity, ( int ), ( const, override ) );
   MOCK_METHOD( int, GetEntityCount, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::Quad<long long>&, GetActiveRegion, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetTileWidth, ( ), ( const, override ) );
   MOCK_METHOD( long long, GetTileHeight, ( ), ( const, override ) );
   MOCK_METHOD( int, GetHorizontalTiles, ( ), ( const, override ) );
   MOCK_METHOD( int, GetVerticalTiles, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ArenaTile&, GetTile, ( long long ), ( const, override ) );
};
