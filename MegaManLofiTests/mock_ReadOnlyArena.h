#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/ReadOnlyArena.h>

class mock_ReadOnlyArena : public MegaManLofi::ReadOnlyArena
{
public:
   MOCK_METHOD( int, GetArenaId, ( ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyEntity>, GetEntity, ( int ), ( const, override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::ReadOnlyEntity>, GetPlayerEntity, ( ), ( const, override ) );
   MOCK_METHOD( int, GetEntityCount, ( ), ( const, override ) );
   MOCK_METHOD( float, GetWidth, ( ), ( const, override ) );
   MOCK_METHOD( float, GetHeight, ( ), ( const, override ) );
   MOCK_METHOD( int, GetHorizontalTiles, ( ), ( const, override ) );
   MOCK_METHOD( int, GetVerticalTiles, ( ), ( const, override ) );
   MOCK_METHOD( const MegaManLofi::ArenaTile&, GetTile, ( int ), ( const, override ) );
   MOCK_METHOD( bool, HasEntity, ( int ), ( const, override ) );
};
