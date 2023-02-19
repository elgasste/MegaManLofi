#include "gtest/gtest.h"

#include <MegaManLofi/Arena.h>
#include <MegaManLofi/ArenaDefs.h>
#include <MegaManLofi/EntityDefs.h>
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
      _entityDefs.reset( new EntityDefs );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _entityFactoryMock.reset( new NiceMock<mock_EntityFactory> );
      _playerMock.reset( new NiceMock<mock_Player> );
      _itemEntityMock.reset( new NiceMock<mock_Entity> );
      _projectileEntityMock.reset( new NiceMock<mock_Entity> );
      _enemyEntityMock.reset( new NiceMock<mock_Entity> );

      _arenaDefs->ArenaId = 11;
      _arenaDefs->HorizontalTiles = 10;
      _arenaDefs->VerticalTiles = 8;
      _arenaDefs->PlayerStartPosition = { 10, 8 };

      _worldDefs->TileWidth = 2;
      _worldDefs->TileHeight = 2;

      _entityDefs->CollisionPayloadMap[3] = { 10, 0 };  // item gives 10 health
      _entityDefs->CollisionPayloadMap[4] = { -10, 0 }; // projectile takes 10 health
      _entityDefs->CollisionPayloadMap[5] = { -5, 0 };  // enemy takes 5 health

      for ( int i = 0; i < _arenaDefs->HorizontalTiles * _arenaDefs->VerticalTiles; i++ )
      {
         _arenaDefs->Tiles.push_back( { true, true, true, true } );
      }

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );

      auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
      ON_CALL( *_entityFactoryMock, CreateEntity( _, _, _ ) ).WillByDefault( Return( entityMock ) );

      ON_CALL( *_itemEntityMock, GetEntityType() ).WillByDefault( Return( EntityType::Item ) );
      ON_CALL( *_itemEntityMock, GetUniqueId() ).WillByDefault( Return( 11 ) );
      ON_CALL( *_itemEntityMock, GetEntityMetaId() ).WillByDefault( Return( 3 ) );
      ON_CALL( *_itemEntityMock, GetVelocityX() ).WillByDefault( Return( 0.0f ) );
      ON_CALL( *_projectileEntityMock, GetEntityType() ).WillByDefault( Return( EntityType::Projectile ) );
      ON_CALL( *_projectileEntityMock, GetEntityMetaId() ).WillByDefault( Return( 4 ) );
      ON_CALL( *_projectileEntityMock, GetVelocityX() ).WillByDefault( Return( -800.0f ) );
      ON_CALL( *_enemyEntityMock, GetEntityType() ).WillByDefault( Return( EntityType::Enemy ) );
      ON_CALL( *_enemyEntityMock, GetEntityMetaId() ).WillByDefault( Return( 5 ) );
      ON_CALL( *_enemyEntityMock, GetVelocityX() ).WillByDefault( Return( 200.0f ) );

      _entityHitBox = { 0, 0, 10, 10 };
      ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( _entityHitBox ) );
      ON_CALL( *_itemEntityMock, GetHitBox() ).WillByDefault( ReturnRef( _entityHitBox ) );
      ON_CALL( *_projectileEntityMock, GetHitBox() ).WillByDefault( ReturnRef( _entityHitBox ) );
      ON_CALL( *_enemyEntityMock, GetHitBox() ).WillByDefault( ReturnRef( _entityHitBox ) );
   }

   void BuildArena()
   {
      _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
      _arena->SetPlayerEntity( _playerMock );
      _arena->SetActiveRegion( { 0, 0, 100, 100 } );
   }

protected:
   shared_ptr<ArenaDefs> _arenaDefs;
   shared_ptr<WorldDefs> _worldDefs;
   shared_ptr<EntityDefs> _entityDefs;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_EntityFactory> _entityFactoryMock;
   shared_ptr<mock_Player> _playerMock;

   shared_ptr<mock_Entity> _itemEntityMock;
   shared_ptr<mock_Entity> _projectileEntityMock;
   shared_ptr<mock_Entity> _enemyEntityMock;

   Rectangle<float> _entityHitBox;

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

TEST_F( ArenaTests, Reload_Always_ClearsEntities )
{
   BuildArena();

   _arena->AddEntity( shared_ptr<mock_Entity>( new NiceMock<mock_Entity> ) );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );

   _arena->Reload();

   EXPECT_EQ( _arena->GetEntityCount(), 0 );
}

TEST_F( ArenaTests, Reload_Always_RaisesArenaEntitiesClearedEvent )
{
   BuildArena();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitiesCleared ) );

   _arena->Reload();
}

TEST_F( ArenaTests, Reload_Always_SetsPlayerEntityToNull )
{
   BuildArena();

   _arena->Reload();

   EXPECT_EQ( _arena->GetPlayerEntity(), nullptr );
}

TEST_F( ArenaTests, Reset_Always_ClearsEntities )
{
   BuildArena();

   _arena->AddEntity( shared_ptr<mock_Entity>( new NiceMock<mock_Entity> ) );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );

   _arena->Reset();

   EXPECT_EQ( _arena->GetEntityCount(), 0 );
}

TEST_F( ArenaTests, Reset_Always_RaisesArenaEntitiesClearedEvent )
{
   BuildArena();

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitiesCleared ) );

   _arena->Reset();
}

TEST_F( ArenaTests, Reset_Always_SetsPlayerEntityToNull )
{
   BuildArena();

   _arena->Reset();

   EXPECT_EQ( _arena->GetPlayerEntity(), nullptr );
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

TEST_F( ArenaTests, CheckSpawnPoints_InactivePointIsNotDecommissioned_SpawnsEntity )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = false;
   _arenaDefs->SpawnPoints[0].EntityMetaId = 5;
   _arenaDefs->SpawnPoints[0].Direction = Direction::Down;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   Coordinate<float> entityPosition;
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 5, _, Direction::Down ) ).WillOnce(
      DoAll( SaveArg<1>( &entityPosition ), Return( entityMock ) ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) );

   _arena->CheckSpawnPoints();

   EXPECT_EQ( entityPosition.Left, 1 );
   EXPECT_EQ( entityPosition.Top, 1 );
   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, CheckSpawnPoints_InactivePointIsBoundToUniqueId_BindsToUniqueId )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = false;
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].EntityMetaId = 5;
   _arenaDefs->SpawnPoints[0].Direction = Direction::Down;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 5, _, Direction::Down ) ).WillOnce( Return( entityMock ) );
   EXPECT_CALL( *entityMock, GetUniqueId() ).WillOnce( Return( 6 ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) );

   _arena->CheckSpawnPoints();

   EXPECT_EQ( _arena->GetSpawnPoint( 0 )->UniqueIdBinding, 6 );
   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, CheckSpawnPoints_InactivePointIsDecommissioned_DoesNotSpawnEntity )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = false;
   _arenaDefs->SpawnPoints[0].IsDecommissioned = true;
   _arenaDefs->SpawnPoints[0].ReSpawnsAtInterval = false;
   BuildArena();

   EXPECT_CALL( *_entityFactoryMock, CreateEntity( _, _, _ ) ).Times( 0 );

   _arena->CheckSpawnPoints();
}

TEST_F( ArenaTests, CheckSpawnPoints_InactivePointBoundToExistingEntityWithUniqueId_DoesNotSpawnEntity )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = false;
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].UniqueIdBinding = 89;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *entityMock, GetUniqueId() ).WillOnce( Return( 89 ) );
   _arena->AddEntity( entityMock );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) ).Times( 0 );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( _, _, _ ) ).Times( 0 );

   _arena->CheckSpawnPoints();
}

TEST_F( ArenaTests, CheckSpawnPoints_ActivePointDoesNotReSpawnAtInterval_DoesNotSpawnEntity )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = true;
   _arenaDefs->SpawnPoints[0].ReSpawnsAtInterval = false;
   BuildArena();

   EXPECT_CALL( *_entityFactoryMock, CreateEntity( _, _, _ ) ).Times( 0 );

   _arena->CheckSpawnPoints();
}

TEST_F( ArenaTests, CheckSpawnPoints_ActivePointReSpawnIntervalNotElapased_DoesNotSpawnEntity )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = true;
   _arenaDefs->SpawnPoints[0].ReSpawnsAtInterval = true;
   _arenaDefs->SpawnPoints[0].ReSpawnIntervalSeconds = 5;
   _arenaDefs->SpawnPoints[0].IntervalElapsedSeconds = 2;
   BuildArena();

   EXPECT_CALL( *_entityFactoryMock, CreateEntity( _, _, _ ) ).Times( 0 );

   _arena->CheckSpawnPoints();
}

TEST_F( ArenaTests, CheckSpawnPoints_ActivePointReSpawnIntervalElapased_SpawnsEntity )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = true;
   _arenaDefs->SpawnPoints[0].ReSpawnsAtInterval = true;
   _arenaDefs->SpawnPoints[0].ReSpawnIntervalSeconds = 5;
   _arenaDefs->SpawnPoints[0].IntervalElapsedSeconds = 4.1f;
   _arenaDefs->SpawnPoints[0].EntityMetaId = 5;
   _arenaDefs->SpawnPoints[0].Direction = Direction::Down;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   Coordinate<float> entityPosition;
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 5, _, Direction::Down ) ).WillOnce(
      DoAll( SaveArg<1>( &entityPosition ), Return( entityMock ) ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) );

   _arena->CheckSpawnPoints();
   EXPECT_EQ( entityPosition.Left, 1 );
   EXPECT_EQ( entityPosition.Top, 1 );
   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, CheckSpawnPoints_ActivePointIsBoundToUniqueId_BindsToUniqueId )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].ArenaPosition = { 1, 1 };
   _arenaDefs->SpawnPoints[0].IsActive = true;
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].ReSpawnsAtInterval = true;
   _arenaDefs->SpawnPoints[0].ReSpawnIntervalSeconds = 5;
   _arenaDefs->SpawnPoints[0].IntervalElapsedSeconds = 4.1f;
   _arenaDefs->SpawnPoints[0].EntityMetaId = 5;
   _arenaDefs->SpawnPoints[0].Direction = Direction::Down;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   EXPECT_CALL( *_entityFactoryMock, CreateEntity( 5, _, Direction::Down ) ).WillOnce( Return( entityMock ) );
   EXPECT_CALL( *entityMock, GetUniqueId() ).WillOnce( Return( 7 ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntitySpawned ) );

   _arena->CheckSpawnPoints();

   EXPECT_EQ( _arena->GetSpawnPoint( 0 )->UniqueIdBinding, 7 );
   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, DeSpawnInactiveEntities_NoInactiveEntities_DoesNotDeSpawnEntities )
{
   Rectangle<float> playerHitBox = { 10, 10, 30, 30 };
   ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( playerHitBox ) );
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock1, GetHealth() ).WillByDefault( Return( 1 ) );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock2, GetHealth() ).WillByDefault( Return( 1 ) );
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

TEST_F( ArenaTests, DeSpawnInactiveEntities_LivingEntitiesFoundOutsideDespawnRegion_DeSpawnsNonPlayerEntities )
{
   Rectangle<float> playerHitBox = { 10, 10, 30, 30 };
   ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( playerHitBox ) );
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock1, GetHealth() ).WillByDefault( Return( 1 ) );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock2, GetHealth() ).WillByDefault( Return( 1 ) );
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

TEST_F( ArenaTests, DeSpawnInactiveEntities_DeadEntitiesFoundInsideDespawnRegion_DeSpawnsNonPlayerEntities )
{
   Rectangle<float> playerHitBox = { 10, 10, 30, 30 };
   ON_CALL( *_playerMock, GetHitBox() ).WillByDefault( ReturnRef( playerHitBox ) );
   BuildArena();
   auto entityMock1 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock1, GetHealth() ).WillByDefault( Return( 1 ) );
   auto entityMock2 = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   ON_CALL( *entityMock2, GetHealth() ).WillByDefault( Return( 0 ) );
   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   Rectangle<float> hitBox1 = { 120, 120, 50, 50 };
   Rectangle<float> hitBox2 = { 100, 100, 10, 10 };
   ON_CALL( *entityMock1, GetHitBox() ).WillByDefault( ReturnRef( hitBox1 ) );
   ON_CALL( *entityMock2, GetHitBox() ).WillByDefault( ReturnRef( hitBox2 ) );

   _arena->SetDeSpawnRegion( { 90, 105, 200, 200 } );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ArenaEntityDeSpawned ) );

   _arena->DeSpawnInactiveEntities();

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 0 ), _playerMock );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock1 );
}

TEST_F( ArenaTests, DeSpawnInactiveEntities_DeadEntityBoundToNonRespawningActiveRegion_DecommissionsSpawnPoint )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].AllowUniqueReSpawn = false;
   _arenaDefs->SpawnPoints[0].UniqueIdBinding = 57;
   _arenaDefs->SpawnPoints[0].Direction = Direction::Down;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   Rectangle<float> entityHitBox = { 120, 120, 50, 50 };
   ON_CALL( *entityMock, GetHitBox() ).WillByDefault( ReturnRef( entityHitBox ) );
   ON_CALL( *entityMock, GetHealth() ).WillByDefault( Return( 0 ) );
   ON_CALL( *entityMock, GetUniqueId() ).WillByDefault( Return( 57 ) );
   _arena->AddEntity( entityMock );

   _arena->DeSpawnInactiveEntities();

   EXPECT_TRUE( _arena->GetSpawnPoint( 0 )->IsDecommissioned );
}

TEST_F( ArenaTests, DeSpawnActiveEntities_DeadEntityBoundToRespawningActiveRegion_DoesNotDecommissionSpawnPoint )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].AllowUniqueReSpawn = true;
   _arenaDefs->SpawnPoints[0].UniqueIdBinding = 57;
   _arenaDefs->SpawnPoints[0].Direction = Direction::Down;
   BuildArena();

   auto entityMock = shared_ptr<mock_Entity>( new NiceMock<mock_Entity> );
   Rectangle<float> entityHitBox = { 120, 120, 50, 50 };
   ON_CALL( *entityMock, GetHitBox() ).WillByDefault( ReturnRef( entityHitBox ) );
   ON_CALL( *entityMock, GetHealth() ).WillByDefault( Return( 0 ) );
   ON_CALL( *entityMock, GetUniqueId() ).WillByDefault( Return( 57 ) );
   _arena->AddEntity( entityMock );

   _arena->DeSpawnInactiveEntities();

   EXPECT_FALSE( _arena->GetSpawnPoint( 0 )->IsDecommissioned );
}

TEST_F( ArenaTests, DetectEntityCollisions_PlayerCollidesWithItem_PlayerTakesPayload )
{
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _playerMock );
   _arena->AddEntity( _itemEntityMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 0 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, 10 );
}

TEST_F( ArenaTests, DetectEntityCollisions_PlayerCollidesWithItemBoundToNonRespawningSpawnPoint_DecommissionsSpawnPoint )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].UniqueIdBinding = 11;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _playerMock );
   _arena->AddEntity( _itemEntityMock );

   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 0 ) ).WillOnce( Return( true ) );

   _arena->DetectEntityCollisions();

   EXPECT_TRUE( _arena->GetSpawnPoint( 0 )->IsDecommissioned );
}

TEST_F( ArenaTests, DetectEntityCollisions_PlayerCollidesWithItemBoundToRespawningSpawnPoint_DoesNotDecommissionSpawnPoint )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].AllowUniqueReSpawn = true;
   _arenaDefs->SpawnPoints[0].UniqueIdBinding = 11;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _playerMock );
   _arena->AddEntity( _itemEntityMock );

   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 0 ) ).WillOnce( Return( true ) );

   _arena->DetectEntityCollisions();

   EXPECT_FALSE( _arena->GetSpawnPoint( 0 )->IsDecommissioned );
}

TEST_F( ArenaTests, DetectEntityCollisions_PlayerCollidesWithEnemy_PlayerTakesHealthPayload )
{
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _playerMock );
   _arena->AddEntity( _enemyEntityMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 200 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, -5 );
}

TEST_F( ArenaTests, DetectEntityCollisions_PlayerCollidesWithFriendlyProjectile_PlayerDoesNotTakeHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = true;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _playerMock );
   _arena->AddEntity( _projectileEntityMock );

   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, _ ) ).Times( 0 );

   _arena->DetectEntityCollisions();
   auto test = true;
}

TEST_F( ArenaTests, DetectEntityCollisions_PlayerCollidesWithUnfriendlyProjectile_PlayerTakesHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = false;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _playerMock );
   _arena->AddEntity( _projectileEntityMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, -800 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, -10 );
}

TEST_F( ArenaTests, DetectEntityCollisions_ItemCollidesWithPlayer_PlayerTakesPayload )
{
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _itemEntityMock );
   _arena->AddEntity( _playerMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 0 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, 10 );
}

TEST_F( ArenaTests, DetectEntityCollisions_ItemBoundToSpawnPointCollidesWithPlayer_DecommissionsSpawnPoint )
{
   _arenaDefs->SpawnPoints.push_back( SpawnPoint() );
   _arenaDefs->SpawnPoints[0].IsBoundToUniqueId = true;
   _arenaDefs->SpawnPoints[0].UniqueIdBinding = 11;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _itemEntityMock );
   _arena->AddEntity( _playerMock );

   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 0 ) ).WillOnce( Return( true ) );

   _arena->DetectEntityCollisions();

   EXPECT_TRUE( _arena->GetSpawnPoint( 0 )->IsDecommissioned );
}

TEST_F( ArenaTests, DetectEntityCollisions_FriendlyProjectileCollidesWithPlayer_PlayerDoesNotTakeHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = true;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _projectileEntityMock );
   _arena->AddEntity( _playerMock );

   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, _ ) ).Times( 0 );

   _arena->DetectEntityCollisions();
}

TEST_F( ArenaTests, DetectEntityCollisions_UnfriendlyProjectileCollidesWithPlayer_PlayerTakesHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = false;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _projectileEntityMock );
   _arena->AddEntity( _playerMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, -800 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, -10 );
}

TEST_F( ArenaTests, DetectEntityCollisions_UnfriendlyProjectileCollidesWithEnemy_EnemyDoesNotTakeHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = false;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _projectileEntityMock );
   _arena->AddEntity( _enemyEntityMock );

   EXPECT_CALL( *_enemyEntityMock, TakeCollisionPayload( _, _ ) ).Times( 0 );

   _arena->DetectEntityCollisions();
}

TEST_F( ArenaTests, DetectEntityCollisions_FriendlyProjectileCollidesWithEnemy_EnemyTakesHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = true;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _projectileEntityMock );
   _arena->AddEntity( _enemyEntityMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_enemyEntityMock, TakeCollisionPayload( _, -800 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, -10 );
}

TEST_F( ArenaTests, DetectEntityCollisions_EnemyCollidesWithPlayer_PlayerTakesHealthPayload )
{
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _enemyEntityMock );
   _arena->AddEntity( _playerMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_playerMock, TakeCollisionPayload( _, 200 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, -5 );
}

TEST_F( ArenaTests, DetectEntityCollisions_EnemyCollidesWithUnfriendlyProjectile_EnemyDoesNotTakeHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = false;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _enemyEntityMock );
   _arena->AddEntity( _projectileEntityMock );

   EXPECT_CALL( *_enemyEntityMock, TakeCollisionPayload( _, _ ) ).Times( 0 );

   _arena->DetectEntityCollisions();
}

TEST_F( ArenaTests, DetectEntityCollisions_EnemyCollidesWithFriendlyProjectile_EnemyTakesHealthPayload )
{
   _entityDefs->ProjectileInfoMap[4].IsFriendly = true;
   _arena.reset( new Arena( _arenaDefs, _worldDefs, _entityDefs, _eventAggregatorMock, _frameRateProviderMock, _entityFactoryMock ) );
   _arena->AddEntity( _enemyEntityMock );
   _arena->AddEntity( _projectileEntityMock );

   EntityCollisionPayload payload;
   EXPECT_CALL( *_enemyEntityMock, TakeCollisionPayload( _, -800 ) ).WillOnce( DoAll( SaveArg<0>( &payload ), Return( true ) ) );

   _arena->DetectEntityCollisions();

   EXPECT_EQ( payload.Health, -10 );
}
