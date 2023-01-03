#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IArena.h>

class mock_Arena : public MegaManLofi::IArena
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
   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IPlayer>, GetMutablePlayer, ( ), ( const, override ) );
   MOCK_METHOD( void, SetPlayer, ( const std::shared_ptr<MegaManLofi::IPlayer> ), ( override ) );
   MOCK_METHOD( void, SetActiveRegion, ( MegaManLofi::Quad<float> ), ( override ) );
   MOCK_METHOD( void, AddEntity, ( const std::shared_ptr<MegaManLofi::IEntity> ), ( override ) );
   MOCK_METHOD( void, RemoveEntity, ( const std::shared_ptr<MegaManLofi::IEntity> ), ( override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IEntity>, GetMutableEntity, ( int ), ( const, override ) );
};
