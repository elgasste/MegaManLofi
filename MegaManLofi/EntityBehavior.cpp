#include "EntityBehavior.h"
#include "IFrameRateProvider.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"
#include "IGameCommandExecutor.h"
#include "Entity.h"
#include "GameCommand.h"
#include "ShootCommandArgs.h"
#include "ShootTargetCommandArgs.h"
#include "ReadOnlyArena.h"
#include "ReadOnlyEntity.h"

using namespace std;
using namespace MegaManLofi;

EntityBehavior::EntityBehavior( const shared_ptr<IFrameRateProvider> frameRateProvider,
                                const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                const shared_ptr<IGameCommandExecutor> commandExecutor ) :
   _frameRateProvider( frameRateProvider ),
   _playerInfoProvider( playerInfoProvider ),
   _arenaInfoProvider( arenaInfoProvider ),
   _commandExecutor( commandExecutor ),
   _entity( nullptr )
{
   Reset();
}

void EntityBehavior::AssignTo( const std::shared_ptr<Entity> entity )
{
   _entity = entity;
}

bool EntityBehavior::HandleCommand( mbc_command command )
{
   if ( MbcVirtualMachine::HandleCommand( command ) )
   {
      return true;
   }

   switch ( command )
   {
      case MBCGET_FRAMESECONDS:
         RegisterFloatFromArg( 0, _frameRateProvider->GetFrameSeconds() );
         return true;

      case MBCGET_PLAYERPOSITIONLEFT:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetArenaPositionLeft() );
         return true;
      case MBCGET_PLAYERPOSITIONTOP:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetArenaPositionTop() );
         return true;
      case MBCGET_PLAYERVELOCITYX:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetVelocityX() );
         return true;
      case MBCGET_PLAYERVELOCITYY:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetVelocityY() );
         return true;
      case MBCGET_PLAYERDIRECTION:
         RegisterIntFromArg( 0, (int)_playerInfoProvider->GetPlayerEntity()->GetDirection() );
         return true;
      case MBCGET_PLAYERHEALTH:
         RegisterIntFromArg( 0, (int)_playerInfoProvider->GetPlayerEntity()->GetHealth() );
         return true;
      case MBCGET_PLAYERMAXHEALTH:
         RegisterIntFromArg( 0, (int)_playerInfoProvider->GetPlayerEntity()->GetMaxHealth() );
         return true;
      case MBCGET_PLAYERMOVEMENTTYPE:
         RegisterIntFromArg( 0, (int)_playerInfoProvider->GetPlayerEntity()->GetMovementType() );
         return true;
      case MBCGET_PLAYERDAMAGESECONDS:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetDamageInvulnerabilitySeconds() );
         return true;
      case MBCGET_PLAYERISINVULNERABLE:
         RegisterBoolFromArg( 0, _playerInfoProvider->GetPlayerEntity()->IsInvulnerable() );
         return true;
      case MBCGET_PLAYERISKNOCKEDBACK:
         RegisterBoolFromArg( 0, _playerInfoProvider->GetPlayerEntity()->IsKnockedBack() );
         return true;
      case MBCGET_PLAYERKNOCKBACKSECONDS:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetKnockBackSeconds() );
         return true;
      case MBCGET_PLAYERKNOCKBACKVELOCITY:
         RegisterFloatFromArg( 0, _playerInfoProvider->GetPlayerEntity()->GetKnockBackVelocity() );
         return true;

      case MBCGET_POSITIONLEFT:
         RegisterFloatFromArg( 0, _entity->GetArenaPositionLeft() );
         return true;
      case MBCGET_POSITIONTOP:
         RegisterFloatFromArg( 0, _entity->GetArenaPositionTop() );
         return true;
      case MBCGET_VELOCITYX:
         RegisterFloatFromArg( 0, _entity->GetVelocityX() );
         return true;
      case MBCGET_VELOCITYY:
         RegisterFloatFromArg( 0, _entity->GetVelocityY() );
         return true;
      case MBCGET_DIRECTION:
         RegisterIntFromArg( 0, (int)_entity->GetDirection() );
         return true;
      case MBCGET_HEALTH:
         RegisterIntFromArg( 0, (int)_entity->GetHealth() );
         return true;
      case MBCGET_MAXHEALTH:
         RegisterIntFromArg( 0, (int)_entity->GetMaxHealth() );
         return true;
      case MBCGET_MOVEMENTTYPE:
         RegisterIntFromArg( 0, (int)_entity->GetMovementType() );
         return true;
      case MBCGET_DAMAGESECONDS:
         RegisterFloatFromArg( 0, _entity->GetDamageInvulnerabilitySeconds() );
         return true;
      case MBCGET_ISINVULNERABLE:
         RegisterBoolFromArg( 0, _entity->IsInvulnerable() );
         return true;
      case MBCGET_ISKNOCKEDBACK:
         RegisterBoolFromArg( 0, _entity->IsKnockedBack() );
         return true;
      case MBCGET_KNOCKBACKSECONDS:
         RegisterFloatFromArg( 0, _entity->GetKnockBackSeconds() );
         return true;
      case MBCGET_KNOCKBACKVELOCITY:
         RegisterFloatFromArg( 0, _entity->GetKnockBackVelocity() );
         return true;

      case MBCGET_ARENAENTITYCOUNT:
         RegisterIntFromArg( 0, _arenaInfoProvider->GetActiveArena()->GetEntityCount() );
         return true;
      case MBCGET_ARENAENTITYTYPE:
         RegisterIntFromArg( 1, (int)_arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetEntityType() );
         return true;
      case MBCGET_ARENAENTITYPOSITIONLEFT:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetArenaPositionLeft() );
         return true;
      case MBCGET_ARENAENTITYPOSITIONTOP:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetArenaPositionTop() );
         return true;
      case MBCGET_ARENAENTITYVELOCITYX:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetVelocityX() );
         return true;
      case MBCGET_ARENAENTITYVELOCITYY:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetVelocityY() );
         return true;
      case MBCGET_ARENAENTITYDIRECTION:
         RegisterIntFromArg( 1, (int)_arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetDirection() );
         return true;
      case MBCGET_ARENAENTITYHEALTH:
         RegisterIntFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetHealth() );
         return true;
      case MBCGET_ARENAENTITYMAXHEALTH:
         RegisterIntFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetMaxHealth() );
         return true;
      case MBCGET_ARENAENTITYMOVEMENTTYPE:
         RegisterIntFromArg( 1, (int)_arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetMovementType() );
         return true;
      case MBCGET_ARENAENTITYDAMAGESECONDS:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetDamageInvulnerabilitySeconds() );
         return true;
      case MBCGET_ARENAENTITYISINVULNERABLE:
         RegisterBoolFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->IsInvulnerable() );
         return true;
      case MBCGET_ARENAENTITYISKNOCKEDBACK:
         RegisterBoolFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->IsKnockedBack() );
         return true;
      case MBCGET_ARENAENTITYKNOCKBACKSECONDS:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetKnockBackSeconds() );
         return true;
      case MBCGET_ARENAENTITYKNOCKBACKVELOCITY:
         RegisterFloatFromArg( 1, _arenaInfoProvider->GetActiveArena()->GetEntity( MBC_PARSE_ARG0( _currentInstruction ) )->GetKnockBackVelocity() );
         return true;

      case MBCSET_VELOCITYX:
         _entity->SetVelocityX( _floatRegisters[MBC_PARSE_ARG0( _currentInstruction )] );
         return true;
      case MBCSET_VELOCITYY:
         _entity->SetVelocityY( _floatRegisters[MBC_PARSE_ARG0( _currentInstruction )] );
         return true;
      case MBCSET_DIRECTION:
         _entity->SetDirection( (Direction)_intRegisters[MBC_PARSE_ARG0( _currentInstruction )] );
         return true;

      case MBCDO_SHOOT:
         _commandExecutor->ExecuteCommand( GameCommand::Shoot, shared_ptr<ShootCommandArgs>( new ShootCommandArgs( _entity ) ) );
         return true;
      case MBCDO_SHOOTTARGET:
         ShootTarget();
         return true;

      default:
         return false;
   }
}

void EntityBehavior::RegisterFloatFromArg( int argNum, float val )
{
   switch ( argNum )
   {
      case 0: _floatRegisters[MBC_PARSE_ARG0( _currentInstruction )] = val; break;
      case 1: _floatRegisters[MBC_PARSE_ARG1( _currentInstruction )] = val; break;
      case 2: _floatRegisters[MBC_PARSE_ARG2( _currentInstruction )] = val; break;
      case 3: _floatRegisters[MBC_PARSE_ARG3( _currentInstruction )] = val; break;
      default: break;
   }
}

void EntityBehavior::RegisterIntFromArg( int argNum, int val )
{
   switch ( argNum )
   {
      case 0: _intRegisters[MBC_PARSE_ARG0( _currentInstruction )] = val; break;
      case 1: _intRegisters[MBC_PARSE_ARG1( _currentInstruction )] = val; break;
      case 2: _intRegisters[MBC_PARSE_ARG2( _currentInstruction )] = val; break;
      case 3: _intRegisters[MBC_PARSE_ARG3( _currentInstruction )] = val; break;
      default: break;
   }
}

void EntityBehavior::RegisterBoolFromArg( int argNum, bool val )
{
   switch ( argNum )
   {
      case 0: _intRegisters[MBC_PARSE_ARG0( _currentInstruction )] = val ? 1 : 0; break;
      case 1: _intRegisters[MBC_PARSE_ARG1( _currentInstruction )] = val ? 1 : 0; break;
      case 2: _intRegisters[MBC_PARSE_ARG2( _currentInstruction )] = val ? 1 : 0; break;
      case 3: _intRegisters[MBC_PARSE_ARG3( _currentInstruction )] = val ? 1 : 0; break;
      default: break;
   }
}

void EntityBehavior::ShootTarget() const
{
   auto targetLeft = _floatRegisters[ MBC_PARSE_ARG0( _currentInstruction ) ];
   auto targetTop = _floatRegisters[ MBC_PARSE_ARG1( _currentInstruction ) ];
   _commandExecutor->ExecuteCommand( GameCommand::ShootTarget,
                                     shared_ptr<ShootTargetCommandArgs>( new ShootTargetCommandArgs( _entity, { targetLeft, targetTop } ) ) );
}
