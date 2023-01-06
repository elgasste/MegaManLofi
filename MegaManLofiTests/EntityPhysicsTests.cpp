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
      _entityMock.reset( new NiceMock<mock_Entity> );

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );

      ON_CALL( *_entityMock, GetMaxGravityVelocity() ).WillByDefault( Return( 1'000.0f ) );
      ON_CALL( *_entityMock, GetGravityAccelerationPerSecond() ).WillByDefault( Return( 100.0f ) );

      ON_CALL( *_stageMock, GetMutableActiveArena() ).WillByDefault( Return( _arenaMock ) );
      ON_CALL( *_arenaMock, GetEntityCount() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_arenaMock, GetMutableEntity( 0 ) ).WillByDefault( Return( _entityMock ) );
   }

   void BuildPhysics()
   {
      _physics.reset( new EntityPhysics( _frameRateProviderMock, _frameActionRegistryMock ) );
      _physics->AssignTo( _stageMock );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_FrameActionRegistry> _frameActionRegistryMock;
   shared_ptr<mock_Stage> _stageMock;
   shared_ptr<mock_Arena> _arenaMock;
   shared_ptr<mock_Entity> _entityMock;

   shared_ptr<EntityPhysics> _physics;
};

TEST_F( EntityPhysicsTests, Tick_ItemNotAtTerminalVelocity_AppliesGravity )
{
   BuildPhysics();
   ON_CALL( *_entityMock, GetEntityType() ).WillByDefault( Return( EntityType::Item ) );
   ON_CALL( *_entityMock, GetVelocityY() ).WillByDefault( Return( 100.0f ) );

   EXPECT_CALL( *_entityMock, SetVelocityY( 200.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_ItemAtTerminalVelocity_DoesNotApplyExtraGravity )
{
   BuildPhysics();
   ON_CALL( *_entityMock, GetEntityType() ).WillByDefault( Return( EntityType::Item ) );
   ON_CALL( *_entityMock, GetVelocityY() ).WillByDefault( Return( 1000.0f ) );

   EXPECT_CALL( *_entityMock, SetVelocityY( 1000.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerMovingUpwardAndNotJumping_StopsUpwardVelocity )
{
   BuildPhysics();
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( false ) );
   ON_CALL( *_entityMock, GetEntityType() ).WillByDefault( Return( EntityType::Player ) );
   ON_CALL( *_entityMock, GetVelocityY() ).WillByDefault( Return( -1000.0f ) );

   EXPECT_CALL( *_entityMock, SetVelocityY( 0.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerMovingUpwardAndJumping_AppliesGravity )
{
   BuildPhysics();
   ON_CALL( *_frameActionRegistryMock, ActionFlagged( FrameAction::PlayerJumping ) ).WillByDefault( Return( true ) );
   ON_CALL( *_entityMock, GetEntityType() ).WillByDefault( Return( EntityType::Player ) );
   ON_CALL( *_entityMock, GetVelocityY() ).WillByDefault( Return( -200.0f ) );

   EXPECT_CALL( *_entityMock, SetVelocityY( -100.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerNotMovingUpwardNotAtTerminalVelocity_AppliesGravity )
{
   BuildPhysics();
   ON_CALL( *_entityMock, GetEntityType() ).WillByDefault( Return( EntityType::Player ) );
   ON_CALL( *_entityMock, GetVelocityY() ).WillByDefault( Return( 100.0f ) );

   EXPECT_CALL( *_entityMock, SetVelocityY( 200.0f ) );

   _physics->Tick();
}

TEST_F( EntityPhysicsTests, Tick_PlayerNotMovingUpwardAtTerminalVelocity_DoesNotApplyExtraGravity )
{
   BuildPhysics();
   ON_CALL( *_entityMock, GetEntityType() ).WillByDefault( Return( EntityType::Player ) );
   ON_CALL( *_entityMock, GetVelocityY() ).WillByDefault( Return( 1000.0f ) );

   EXPECT_CALL( *_entityMock, SetVelocityY( 1000.0f ) );

   _physics->Tick();
}
