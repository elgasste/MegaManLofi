#include "gtest/gtest.h"

#include <MegaManLofi/EntityBehavior.h>

#include "mock_FrameRateProvider.h"
#include "mock_PlayerInfoProvider.h"
#include "mock_ReadOnlyEntity.h"
#include "mock_Entity.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EntityBehaviorTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _playerInfoProviderMock.reset( new NiceMock<mock_PlayerInfoProvider> );
      _entityMock.reset( new NiceMock<mock_Entity> );
      _playerEntityMock.reset( new NiceMock<mock_ReadOnlyEntity> );

      ON_CALL( *_playerInfoProviderMock, GetPlayerEntity() ).WillByDefault( Return( _playerEntityMock ) );

      _behavior.reset( new EntityBehavior( _frameRateProviderMock, _playerInfoProviderMock ) );
      _behavior->AssignTo( _entityMock );
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
   shared_ptr<mock_Entity> _entityMock;
   shared_ptr<mock_ReadOnlyEntity> _playerEntityMock;

   shared_ptr<EntityBehavior> _behavior;
};

TEST_F( EntityBehaviorTests, Tick_GetFrameSecondsCommand_GetsFrameSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_FRAMESECS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_frameRateProviderMock, GetFrameSeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerPositionLeftCommand_GetsPlayerPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRPOSL << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerPositionTopCommand_GetsPlayerPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRPOST << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerVelocityXCommand_GetsPlayerVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRVELX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerVelocityYCommand_GetsPlayerVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRVELY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerDirectionCommand_GetsPlayerDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRDIR << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerHealthCommand_GetsPlayerHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerMaxHealthCommand_GetsPlayerMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRMXHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerMovementTypeCommand_GetsPlayerMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_PLRMVM << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPositionLeftCommand_GetsPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTPOSL << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPositionTopCommand_GetsPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTPOST << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetVelocityXCommand_GetsVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTVELX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetVelocityYCommand_GetsVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTVELY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetDirectionCommand_GetsDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTDIR << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetHealthCommand_GetsHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetMaxHealthCommand_GetsMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTMXHTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetMovementTypeCommand_GetsMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_ENTMVM << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_SetVelocityXCommand_SetsVelocityX )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 3 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 6.0f ),
      (mbc_instruction)( MBCSET_ENTVELX << MBC_CMD_SHIFT | 3 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_entityMock, SetVelocityX( 6.0f ) );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_SetVelocityYCommand_SetsVelocityY )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 3.0f ),
      (mbc_instruction)( MBCSET_ENTVELY << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_entityMock, SetVelocityY( 3.0f ) );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_SetDirectionCommand_SetsDirection )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGI << MBC_CMD_SHIFT | 2 << MBC_ARG0_SHIFT ),
      (mbc_instruction)( Direction::Right ),
      (mbc_instruction)( MBCSET_ENTDIR << MBC_CMD_SHIFT | 2 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_entityMock, SetDirection( Direction::Right ) );

   _behavior->Tick();
}
