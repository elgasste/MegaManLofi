#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArena.h>

class mock_Arena : public MegaManLofi::IArena
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
   MOCK_METHOD( bool, HasEntity, ( int ), ( const, override ) );
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IPlayer>, GetMutablePlayer, ( ), ( const, override ) );
   MOCK_METHOD( void, SetPlayer, ( const std::shared_ptr<MegaManLofi::IPlayer> ), ( override ) );
   MOCK_METHOD( void, SetActiveRegion, ( MegaManLofi::Quad<long long> ), ( override ) );
   MOCK_METHOD( void, AddEntity, ( const std::shared_ptr<MegaManLofi::IEntity> ), ( override ) );
   MOCK_METHOD( void, RemoveEntity, ( const std::shared_ptr<MegaManLofi::IEntity> ), ( override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IEntity>, GetMutableEntity, ( int ), ( const, override ) );
};
