#include "gtest/gtest.h"

#include <MegaManLofi/EntityPhysics.h>

#include "mock_FrameRateProvider.h"
#include "mock_FrameActionRegistry.h"
#include "mock_Stage.h"
#include "mock_Arena.h"
#include "mock_Entity.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EntityPhysicsTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _frameActionRegistryMock.reset( new NiceMock<mock_FrameActionRegistry> );
      _stageMock.reset( new NiceMock<mock_Stage> );
      _arenaMock.reset( new NiceMock<mock_Arena> );
      _playerMock.reset( new NiceMock<mock_Entity> );
      _itemMock.reset( new NiceMock<mock_Entity> );
      _enemyMock.reset( new NiceMock<mock_Entity> );
      _projectileMock.reset( new NiceMock<mock_Entity> );

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );

      ON_CALL( *_playerMock, GetEntityType() ).WillByDefault( Return( EntityType::Player ) );
      ON_CALL( *_playerMock, GetMaxGravityVelocity() ).WillByDefault( Return( 1'000.0f ) );
      ON_CALL( *_playerMock, GetGravityAccelerationPerSecond() ).WillByDefault( Return( 100.0f ) );
      ON_CALL( *_playerMock, GetFrictionDecelerationPerSecond() ).WillByDefault( Return( 2.0f ) );

      ON_CALL( *_itemMock, GetEntityType() ).WillByDefault( Return( EntityType::Item ) );
      ON_CALL( *_itemMock, GetMaxGravityVelocity() ).WillByDefault( Return( 1'000.0f ) );
      ON_CALL( *_itemMock, GetGravityAccelerationPerSecond() ).WillByDefault( Return( 100.0f ) );
      ON_CALL( *_itemMock, GetFrictionDecelerationPerSecond() ).WillByDefault( Return( 2.0f ) );

      ON_CALL( *_enemyMock, GetEntityType() ).WillByDefault( Return( EntityType::Enemy ) );
      ON_CALL( *_enemyMock, GetMaxGravityVelocity() ).WillByDefault( Return( 1'000.0f ) );
      ON_CALL( *_enemyMock, GetGravityAccelerationPerSecond() ).WillByDefault( Return( 100.0f ) );
      ON_CALL( *_enemyMock, GetFrictionDecelerationPerSecond() ).WillByDefault( Return( 2.0f ) );

      ON_CALL( *_projectileMock, GetEntityType() ).WillByDefault( Return( EntityType::Projectile ) );
      ON_CALL( *_projectileMock, GetMaxGravityVelocity() ).WillByDefault( Return( 1'000.0f ) );
      ON_CALL( *_projectileMock, GetGravityAccelerationPerSecond() ).WillByDefault( Return( 100.0f ) );
      ON_CALL( *_projectileMock, GetFrictionDecelerationPerSecond() ).WillByDefault( Return( 0.0f ) );

      ON_CALL( *_stageMock, GetMutableActiveArena() ).WillByDefault( Return( _arenaMock ) );
      ON_CALL( *_arenaMock, GetEntityCount() ).WillByDefault( Return( 1 ) );
   }

   void BuildPhysics( shared_ptr<mock_Entity> entity )
   {
      ON_CALL( *_arenaMock, GetMutableEntity( 0 ) ).WillByDefault( Return( entity ) );
      _physics.reset( new EntityPhysics( _frameRateProviderMock, _frameActionRegistryMock ) );
      _physics->AssignTo( _stageMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_Stage> _stageMock;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_Entity> _playerMock;
   shared_ptr<mock_Entity> _itemMock;
   shared_ptr<mock_Entity> _enemyMock;
   shared_ptr<mock_Entity> _projectileMock;

   shared_ptr<EntityPhysics> _physics;
};

TEST_F( EntityPhysicsTests, Tick_PlayerNotAtTerminalVelocity_AppliesGravity )
{
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 100.0f ) );
   BuildPhysics( _playerMock );

   EXPECT_CALL( *_playerMock, SetVelocityY( 200.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerAtTerminalVelocity_DoesNotApplyExtraGravity )
{
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( 1000.0f ) );
   BuildPhysics( _playerMock );

   EXPECT_CALL( *_playerMock, SetVelocityY( 1000.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_ItemNotAtTerminalVelocity_AppliesGravity )
{
   ON_CALL( *_itemMock, GetVelocityY() ).WillByDefault( Return( 100.0f ) );
   BuildPhysics( _itemMock );

   EXPECT_CALL( *_itemMock, SetVelocityY( 200.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_ItemAtTerminalVelocity_DoesNotApplyExtraGravity )
{
   ON_CALL( *_itemMock, GetVelocityY() ).WillByDefault( Return( 1000.0f ) );
   BuildPhysics( _itemMock );

   EXPECT_CALL( *_itemMock, SetVelocityY( 1000.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_EnemyNotAtTerminalVelocity_AppliesGravity )
{
   ON_CALL( *_enemyMock, GetVelocityY() ).WillByDefault( Return( 100.0f ) );
   BuildPhysics( _enemyMock );

   EXPECT_CALL( *_enemyMock, SetVelocityY( 200.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_EnemyAtTerminalVelocity_DoesNotApplyExtraGravity )
{
   ON_CALL( *_enemyMock, GetVelocityY() ).WillByDefault( Return( 1000.0f ) );
   BuildPhysics( _enemyMock );

   EXPECT_CALL( *_enemyMock, SetVelocityY( 1000.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_Projectile_DoesNotApplyGravity )
{
   BuildPhysics( _projectileMock );

   EXPECT_CALL( *_projectileMock, SetVelocityY( _ ) ).Times( 0 );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerMovingUpwardAndNotJumping_StopsUpwardVelocity )
{
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( false ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -1000.0f ) );
   BuildPhysics( _playerMock );

   EXPECT_CALL( *_playerMock, SetVelocityY( 0.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerMovingUpwardAndJumping_AppliesGravity )
{
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( true ) );
   ON_CALL( *_playerMock, GetVelocityY() ).WillByDefault( Return( -200.0f ) );
   BuildPhysics( _playerMock );

   EXPECT_CALL( *_playerMock, SetVelocityY( -100.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerEntityWasPushed_DoesNotApplyFriction )
{
   BuildPhysics( _playerMock );

   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerPushed ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_playerMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_Projectile_DoesNotApplyFriction )
{
   BuildPhysics( _projectileMock );

   EXPECT_CALL( *_projectileMock, SetVelocityX( _ ) ).Times( 0 );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_ItemIsMovingLeft_SlowsDownCorrectly )
{
   ON_CALL( *_itemMock, GetVelocityX() ).WillByDefault( Return( -4.0f ) );
   BuildPhysics( _itemMock );

   EXPECT_CALL( *_itemMock, SetVelocityX( -2 ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_EnemyIsMovingLeft_SlowsDownCorrectly )
{
   ON_CALL( *_enemyMock, GetVelocityX() ).WillByDefault( Return( -4.0f ) );
   BuildPhysics( _enemyMock );

   EXPECT_CALL( *_enemyMock, SetVelocityX( -2 ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_ItemIsMovingRight_SlowsDownCorrectly )
{
   ON_CALL( *_itemMock, GetVelocityX() ).WillByDefault( Return( 4.0f ) );
   BuildPhysics( _itemMock );

   EXPECT_CALL( *_itemMock, SetVelocityX( 2 ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_EnemyIsMovingRight_SlowsDownCorrectly )
{
   ON_CALL( *_enemyMock, GetVelocityX() ).WillByDefault( Return( 4.0f ) );
   BuildPhysics( _enemyMock );

   EXPECT_CALL( *_enemyMock, SetVelocityX( 2 ) );

   _physics->Tick();
}
