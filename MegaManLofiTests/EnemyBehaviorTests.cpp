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
   auto instruction = (mbc_instruction)( MBCGET_PLAYERPOSL << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerPositionTopCommand_GetsPlayerPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERPOST << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerVelocityXCommand_GetsPlayerVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERVELX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EnemyBehaviorTests, Tick_GetPlayerVelocityYCommand_GetsPlayerVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERVELY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityY() );

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
