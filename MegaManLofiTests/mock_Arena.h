#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/Arena.h>

class mock_Arena : public MegaManLofi::Arena
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

   MOCK_METHOD( void, Reset, ( ), ( override ) );
   MOCK_METHOD( void, Clear, ( ), ( override ) );
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::Entity>, GetMutableEntity, ( int ), ( const, override ) );
   MOCK_METHOD( void, SetPlayerEntity, ( const std::shared_ptr<MegaManLofi::Entity> ), ( override ) );
   MOCK_METHOD( void, SetActiveRegion, ( MegaManLofi::Rectangle<float> ), ( override ) );
   MOCK_METHOD( void, SetDeSpawnRegion, ( MegaManLofi::Rectangle<float> ), ( override ) );
   MOCK_METHOD( void, AddEntity, ( const std::shared_ptr<MegaManLofi::Entity> ), ( override ) );
   MOCK_METHOD( void, RemoveEntity, ( const std::shared_ptr<MegaManLofi::Entity> ), ( override ) );
   MOCK_METHOD( void, AddSpawnPoint, ( const std::shared_ptr<MegaManLofi::SpawnPoint> ), ( override ) );
   MOCK_METHOD( void, CheckSpawnPoints, ( ), ( override ) );
   MOCK_METHOD( void, DeSpawnInactiveEntities, ( ), ( override ) );
};
