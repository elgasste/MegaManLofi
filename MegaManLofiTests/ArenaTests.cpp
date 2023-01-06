#include "gtest/gtest.h"

#include <MegaManLofi/Arena.h>
#include <MegaManLofi/ArenaDefs.h>
#include <MegaManLofi/WorldDefs.h>
#include <MegaManLofi/FrameAction.h>

#include "mock_GameEventAggregator.h"
#include "mock_FrameRateProvider.h"
#include "mock_Player.h"
#include "mock_FrameActionRegistry.h"
#include "mock_FrameRateProvider.h"
#include "mock_EntityFactory.h"
#include "mock_Entity.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ArenaTests : public Test
{
public:
   void SetUp() override
   {
      _arenaDefs.reset( new ArenaDefs );
      _worldDefs.reset( new WorldDefs );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _entityFactoryMock.reset( new NiceMock<mock_EntityFactory> );
      _playerMock.reset( new NiceMock<mock_Player> );

      _arenaDefs->ArenaId = 11;
      _arenaDefs->HorizontalTiles = 10;
      _arenaDefs->VerticalTiles = 8;
      _arenaDefs->PlayerStartPosition = { 10, 8 };

      _worldDefs->TileWidth = 2;
      _worldDefs->TileHeight = 2;

      for ( int i = 0; i < _arenaDefs->HorizontalTiles * _arenaDefs->VerticalTiles; i++ )
      {
         _arenaDefs->Tiles.push_back( { true, true, true, true } );
      }

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   }

   void BuildArena()
   {
      _arena.reset( new Arena( _arenaDefs, _worldDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
      _arena->SetPlayerEntity( _playerMock );
   }

protected:
   shared_ptr<ArenaDefs> _arenaDefs;
   shared_ptr<WorldDefs> _worldDefs;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_EntityFactory> _entityFactoryMock;
   shared_ptr<mock_Player> _playerMock;

   shared_ptr<Arena> _arena;
};

TEST_F( ArenaTests, Constructor_Always_SetsDefaultInfoBasedOnDefs )
{
   _arenaDefs->Tiles[5] = { false, true, false, true };
   BuildArena();

   EXPECT_EQ( _arena->GetArenaId(), 11 );
   EXPECT_EQ( _arena->GetWidth(), 20 );
   EXPECT_EQ( _arena->GetHeight(), 16 );
   EXPECT_EQ( _arena->GetHorizontalTiles(), 10 );
   EXPECT_EQ( _arena->GetVerticalTiles(), 8 );

   EXPECT_FALSE( _arena->GetTile( 5 ).LeftPassable );
   EXPECT_TRUE( _arena->GetTile( 5 ).UpPassable );
   EXPECT_FALSE( _arena->GetTile( 5 ).RightPassable );
   EXPECT_TRUE( _arena->GetTile( 5 ).DownPassable );
}

TEST_F( ArenaTests, Reset_Always_ClearsEntities )
{
   BuildArena();

   _arena->AddEntity( shared_ptr<mock_Entity>( new NiceMock<mock_Entity> ) );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );

   _arena->Reset();

   EXPECT_EQ( _arena->GetEntityCount(), 1 );
   EXPECT_EQ( _arena->GetEntity( 0 ), _playerMock );
}

TEST_F( ArenaTests, Reset_Always_RaisesArenaEntitiesClearedEvent )
{
   BuildArena();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitiesCleared ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) );

   _arena->Reset();
}

TEST_F( ArenaTests, Clear_Always_ClearsEntitiesAndPlayer )
{
   BuildArena();

   EXPECT_EQ( _arena->GetEntityCount(), 1 );
   EXPECT_EQ( _arena->GetPlayerEntity(), _playerMock );

   _arena->Clear();

   EXPECT_EQ( _arena->GetEntityCount(), 0 );
   EXPECT_EQ( _arena->GetPlayerEntity(), nullptr );
}

TEST_F( ArenaTests, Clear_Always_RaisesArenaEntitiesClearedEvent )
{
   BuildArena();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitiesCleared ) );

   _arena->Clear();
}

TEST_F( ArenaTests, SetPlayerEntity_Always_ResetsPlayerEntityPositionFromDefs )
{
   BuildArena();

   Coordinate<float> position;
   EXPECT_CALL( *_playerMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &position ) );

   _arena->SetPlayerEntity( _playerMock );

   EXPECT_EQ( position.Left, _arenaDefs->PlayerStartPosition.Left );
   EXPECT_EQ( position.Top, _arenaDefs->PlayerStartPosition.Top );
}

TEST_F( ArenaTests, HasEntity_DoesNotHaveEntity_ReturnsFalse )
{
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock1, GetUniqueId() ).WillByDefault( Return( 3 ) );
   ON_CALL( *entityMock2, GetUniqueId() ).WillByDefault( Return( 7 ) );

   _arena->AddEntity( entityMock1 );

   EXPECT_FALSE( _arena->HasEntity( 7 ) );
}

TEST_F( ArenaTests, HasEntity_HasEntity_ReturnsTrue )
{
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock1, GetUniqueId() ).WillByDefault( Return( 3 ) );
   ON_CALL( *entityMock2, GetUniqueId() ).WillByDefault( Return( 7 ) );

   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   EXPECT_TRUE( _arena->HasEntity( 7 ) );
}

TEST_F( ArenaTests, AddEntity_EntityIsNotInList_AddsEntity )
{
   BuildArena();
   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );

   _arena->AddEntity( entityMock );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, AddEntity_EntityIsNotInList_RaisesArenaEntitySpawnedEvent )
{
   BuildArena();
   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) );

   _arena->AddEntity( entityMock );
}

TEST_F( ArenaTests, AddEntity_EntityIsAlreadyInList_DoesNotAddEntity )
{
   BuildArena();
   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock );

   _arena->AddEntity( entityMock );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, AddEntity_EntityIsAlreadyInList_DoesNotRaiseArenaEntitySpawnedEvent )
{
   BuildArena();
   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) ).Times( 0 );

   _arena->AddEntity( entityMock );
}

TEST_F( ArenaTests, RemoveEntity_EntityIsNotInList_DoesNotRemoveEntity )
{
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock1 );

   _arena->RemoveEntity( entityMock2 );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock1 );
}

TEST_F( ArenaTests, RemoveEntity_EntityIsNotInList_DoesNotRaiseArenaEntityDeSpawnedEvent )
{
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock1 );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntityDeSpawned ) ).Times( 0 );

   _arena->RemoveEntity( entityMock2 );
}

TEST_F( ArenaTests, RemoveEntity_EntityIsInList_RemovesEntity )
{
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   _arena->RemoveEntity( entityMock1 );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock2 );
}

TEST_F( ArenaTests, RemoveEntity_EntityIsInList_RaisesArenaEntityDeSpawnedEvent )
{
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntityDeSpawned ) );

   _arena->RemoveEntity( entityMock2 );
}

TEST_F( ArenaTests, DeSpawnInactiveEntities_NoInactiveEntities_DoesNotDeSpawnEntities )
{
   Rectangle<float> playerHitBox = { 10, 10, 30, 30 };
   ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( playerHitBox ) );
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   Rectangle<float> hitBox1 = { 20, 20, 50, 50 };
   Rectangle<float> hitBox2 = { 100, 100, 10, 10 };
   ON_CALL( *entityMock1, GetHitBox() ).WillByDefault( ReturnRef( hitBox1 ) );
   ON_CALL( *entityMock2, GetHitBox() ).WillByDefault( ReturnRef( hitBox2 ) );
   
   _arena->SetDeSpawnRegion( { 0, 0, 200, 200 } );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntityDeSpawned ) ).Times( 0 );

   _arena->DeSpawnInactiveEntities();

   EXPECT_EQ( _arena->GetEntityCount(), 3 );
}

TEST_F( ArenaTests, DeSpawnInactiveEntities_InactiveEntitiesFound_DeSpawnsNonPlayerEntities )
{
   Rectangle<float> playerHitBox = { 10, 10, 30, 30 };
   ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( playerHitBox ) );
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   Rectangle<float> hitBox1 = { 20, 20, 50, 50 };
   Rectangle<float> hitBox2 = { 100, 100, 10, 10 };
   ON_CALL( *entityMock1, GetHitBox() ).WillByDefault( ReturnRef( hitBox1 ) );
   ON_CALL( *entityMock2, GetHitBox() ).WillByDefault( ReturnRef( hitBox2 ) );

   _arena->SetDeSpawnRegion( { 90, 105, 200, 200 } );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntityDeSpawned ) );

   _arena->DeSpawnInactiveEntities();

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 0 ), _playerMock );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock2 );
}
