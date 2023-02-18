#include "gtest/gtest.h"

#include <MegaManLofi/EntityBehavior.h>
#include <MegaManLofi/GameCommand.h>
#include <MegaManLofi/ShootCommandArgs.h>
#include <MegaManLofi/ShootTargetCommandArgs.h>

#include "mock_FrameRateProvider.h"
#include "mock_PlayerInfoProvider.h"
#include "mock_ArenaInfoProvider.h"
#include "mock_GameCommandExecutor.h"
#include "mock_ReadOnlyEntity.h"
#include "mock_Entity.h"
#include "mock_ReadOnlyArena.h"

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
      _arenaInfoProviderMock.reset( new NiceMock<mock_ArenaInfoProvider> );
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );
      _entityMock.reset( new NiceMock<mock_Entity> );
      _playerEntityMock.reset( new NiceMock<mock_ReadOnlyEntity> );
      _arenaMock.reset( new NiceMock<mock_ReadOnlyArena> );
      _arenaEntityMock.reset( new NiceMock<mock_ReadOnlyEntity> );

      ON_CALL( *_playerInfoProviderMock, GetPlayerEntity() ).WillByDefault( Return( _playerEntityMock ) );

      ON_CALL( *_arenaInfoProviderMock, GetActiveArena() ).WillByDefault( Return( _arenaMock ) );
      ON_CALL( *_arenaMock, GetEntity( 0 ) ).WillByDefault( Return( _arenaEntityMock ) );

      _behavior.reset( new EntityBehavior( _frameRateProviderMock, _playerInfoProviderMock, _arenaInfoProviderMock, _commandExecutorMock ) );
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
   shared_ptr<mock_ArenaInfoProvider> _arenaInfoProviderMock;
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;
   shared_ptr<mock_Entity> _entityMock;
   shared_ptr<mock_ReadOnlyEntity> _playerEntityMock;
   shared_ptr<mock_ReadOnlyArena> _arenaMock;
   shared_ptr<mock_ReadOnlyEntity> _arenaEntityMock;

   shared_ptr<EntityBehavior> _behavior;
};

TEST_F( EntityBehaviorTests, Tick_GetFrameSecondsCommand_GetsFrameSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_FRAMESECONDS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_frameRateProviderMock, GetFrameSeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerPositionLeftCommand_GetsPlayerPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERPOSITIONLEFT << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerPositionTopCommand_GetsPlayerPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERPOSITIONTOP << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerVelocityXCommand_GetsPlayerVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERVELOCITYX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerVelocityYCommand_GetsPlayerVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERVELOCITYY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerDirectionCommand_GetsPlayerDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERDIRECTION << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerHealthCommand_GetsPlayerHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERHEALTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerMaxHealthCommand_GetsPlayerMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERMAXHEALTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerMovementTypeCommand_GetsPlayerMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERMOVEMENTTYPE << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerDamageSecondsCommand_GetsPlayerDamageSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERDAMAGESECONDS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetDamageInvulnerabilitySeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerIsInvulnerable_GetsPlayerIsInvulnerable )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERISINVULNERABLE << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, IsInvulnerable() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerIsKnockedBack_GetsPlayerIsKnockedBack )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERISKNOCKEDBACK << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, IsKnockedBack() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPlayerKnockBackSecondsCommand_GetsPlayerKnockBackSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_PLAYERKNOCKBACKSECONDS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_playerEntityMock, GetKnockBackSeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityCount_GetsArenaEntityCount )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYCOUNT << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntityCount() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityType_GetsArenaEntityType )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYTYPE << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetEntityType() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityPositionLeftCount_GetsArenaEntityPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYPOSITIONLEFT << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityPositionTop_GetsArenaEntityPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYPOSITIONTOP << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityVelocityX_GetsArenaEntityVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYVELOCITYX << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityVelocityY_GetsArenaEntityVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYVELOCITYY << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityDirection_GetsArenaEntityDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYDIRECTION << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityHealth_GetsArenaEntityHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYHEALTH << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityMaxHealth_GetsArenaEntityMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYMAXHEALTH << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityMovementType_GetsArenaEntityMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYMOVEMENTTYPE << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityDamageSeconds_GetsArenaEntityDamageSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYDAMAGESECONDS << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetDamageInvulnerabilitySeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityIsInvulnerable_GetsArenaEntityIsInvulnerable )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYISINVULNERABLE << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, IsInvulnerable() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityIsKnockedBack_GetsArenaEntityIsKnockedBack )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYISKNOCKEDBACK << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, IsKnockedBack() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetArenaEntityKnockBackSeconds_GetsArenaEntityKnockBackSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_ARENAENTITYKNOCKBACKSECONDS << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_arenaMock, GetEntity( 0 ) );
   EXPECT_CALL( *_arenaEntityMock, GetKnockBackSeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPositionLeftCommand_GetsPositionLeft )
{
   auto instruction = (mbc_instruction)( MBCGET_POSITIONLEFT << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetArenaPositionLeft() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetPositionTopCommand_GetsPositionTop )
{
   auto instruction = (mbc_instruction)( MBCGET_POSITIONTOP << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetArenaPositionTop() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetVelocityXCommand_GetsVelocityX )
{
   auto instruction = (mbc_instruction)( MBCGET_VELOCITYX << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetVelocityX() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetVelocityYCommand_GetsVelocityY )
{
   auto instruction = (mbc_instruction)( MBCGET_VELOCITYY << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetVelocityY() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetDirectionCommand_GetsDirection )
{
   auto instruction = (mbc_instruction)( MBCGET_DIRECTION << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetDirection() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetHealthCommand_GetsHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_HEALTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetMaxHealthCommand_GetsMaxHealth )
{
   auto instruction = (mbc_instruction)( MBCGET_MAXHEALTH << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetMaxHealth() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetMovementTypeCommand_GetsMovementType )
{
   auto instruction = (mbc_instruction)( MBCGET_MOVEMENTTYPE << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetMovementType() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetDamageSecondsCommand_GetsDamageSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_DAMAGESECONDS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetDamageInvulnerabilitySeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetIsInvulnerable_GetsIsInvulnerable )
{
   auto instruction = (mbc_instruction)( MBCGET_ISINVULNERABLE << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, IsInvulnerable() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetIsKnockedBack_GetsIsKnockedBack )
{
   auto instruction = (mbc_instruction)( MBCGET_ISKNOCKEDBACK << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, IsKnockedBack() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_GetKnockBackSecondsCommand_GetsKnockBackSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_KNOCKBACKSECONDS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_entityMock, GetKnockBackSeconds() );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_SetVelocityXCommand_SetsVelocityX )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 3 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 6.0f ),
      (mbc_instruction)( MBCSET_VELOCITYX << MBC_CMD_SHIFT | 3 << MBC_ARG0_SHIFT )
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
      (mbc_instruction)( MBCSET_VELOCITYY << MBC_CMD_SHIFT | 0 << MBC_ARG0_SHIFT )
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
      (mbc_instruction)( MBCSET_DIRECTION << MBC_CMD_SHIFT | 2 << MBC_ARG0_SHIFT )
   };
   _behavior->SetInstructions( instructions );

   EXPECT_CALL( *_entityMock, SetDirection( Direction::Right ) );

   _behavior->Tick();
}

TEST_F( EntityBehaviorTests, Tick_ShootCommand_ExecutesShootCommand )
{
   auto instruction = (mbc_instruction)( MBCDO_SHOOT << MBC_CMD_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   shared_ptr<GameCommandArgs> args;
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::Shoot, _ ) ).WillOnce( SaveArg<1>( &args ) );

   _behavior->Tick();

   EXPECT_EQ( _entityMock, static_pointer_cast<ShootCommandArgs>( args )->SourceEntity );
}

TEST_F( EntityBehaviorTests, Tick_ShootTargetCommand_ExecutesShootTargetCommand )
{
   vector<mbc_instruction> instructions
   {
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 1 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 6.0f ),
      (mbc_instruction)( MBCCMD_REGF << MBC_CMD_SHIFT | 2 << MBC_ARG0_SHIFT ),
      GetFloatAsInstruction( 8.0f ),
      (mbc_instruction)( MBCDO_SHOOTTARGET << MBC_CMD_SHIFT | 1 << MBC_ARG0_SHIFT | 2 << MBC_ARG1_SHIFT )
   };
   _behavior->SetInstructions( vector<mbc_instruction> { instructions } );

   shared_ptr<GameCommandArgs> args;
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::ShootTarget, _ ) ).WillOnce( SaveArg<1>( &args ) );

   _behavior->Tick();

   EXPECT_EQ( _entityMock, static_pointer_cast<ShootTargetCommandArgs>( args )->SourceEntity );
   EXPECT_EQ( 6.0f, static_pointer_cast<ShootTargetCommandArgs>( args )->TargetPosition.Left );
   EXPECT_EQ( 8.0f, static_pointer_cast<ShootTargetCommandArgs>( args )->TargetPosition.Top );
}
