#include "gtest/gtest.h"

#include <MegaManLofi/EnemyBehavior.h>

#include "mock_FrameRateProvider.h"
#include "mock_PlayerInfoProvider.h"
#include "mock_ReadOnlyEntity.h"
#include "mock_Entity.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EnemyBehaviorTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _playerInfoProviderMock.reset( new NiceMock<mock_PlayerInfoProvider> );
      _enemyMock.reset( new NiceMock<mock_Entity> );
      _playerEntityMock.reset( new NiceMock<mock_ReadOnlyEntity> );

      ON_CALL( *_playerInfoProviderMock, GetPlayerEntity() ).WillByDefault( Return( _playerEntityMock ) );

      _behavior.reset( new EnemyBehavior( _frameRateProviderMock, _playerInfoProviderMock ) );
      _behavior->AssignTo( _enemyMock );
   }

   mbc_instruction GetFloatAsInstruction( float number )
   {
      mbc_instruction instruction;
      memcpy( &instruction, &number, 4 );
      return instruction;
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_PlayerInfoProvider> _playerInfoProviderMock;
   shared_ptr<mock_Entity> _enemyMock;
   shared_ptr<mock_ReadOnlyEntity> _playerEntityMock;

   shared_ptr<EnemyBehavior> _behavior;
};

TEST_F( EnemyBehaviorTests, Tick_GetFrameSecondsCommand_GetsFrameSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_FRAMESECS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_frameRateProviderMock, GetFrameSeconds() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerPositionLeftCommand_GetsPlayerPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRPOSL << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerPositionTopCommand_GetsPlayerPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRPOST << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerVelocityXCommand_GetsPlayerVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRVELX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerVelocityYCommand_GetsPlayerVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRVELY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerDirectionCommand_GetsPlayerDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRDIR << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerHealthCommand_GetsPlayerHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerMaxHealthCommand_GetsPlayerMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRMXHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerMovementTypeCommand_GetsPlayerMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRMVM << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyPositionLeftCommand_GetsEnemyPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTPOSL << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyPositionTopCommand_GetsEnemyPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTPOST << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyVelocityXCommand_GetsEnemyVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTVELX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyVelocityYCommand_GetsEnemyVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTVELY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyDirectionCommand_GetsEnemyDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTDIR << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyHealthCommand_GetsEnemyHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyMaxHealthCommand_GetsEnemyMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTMXHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetEnemyMovementTypeCommand_GetsEnemyMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTMVM << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_enemyMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_SetEnemyVelocityXCommand_SetsEnemyVelocityX )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 3 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 6.0f ),
      (mbc_instruction)( MBCSET_ENTVELX << MBC_CMD_SHIFT | 3 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_enemyMock, SetVelocityX( 6.0f ) );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_SetEnemyVelocityYCommand_SetsEnemyVelocityY )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 3.0f ),
      (mbc_instruction)( MBCSET_ENTVELY << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_enemyMock, SetVelocityY( 3.0f ) );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_SetEnemyDirectionCommand_SetsEnemyDirection )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGI << MBC_CMD_SHIFT | 2 << MBC_ARG0_SHIFT ),
      (mbc_instruction)( Direction::Right ),
      (mbc_instruction)( MBCSET_ENTDIR << MBC_CMD_SHIFT | 2 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_enemyMock, SetDirection( Direction::Right ) );

   _behavior->Tick();
}
