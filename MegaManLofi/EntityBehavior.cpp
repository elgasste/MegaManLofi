#include "EntityBehavior.h"
#include "IFrameRateProvider.h"
#include "IPlayerInfoProvider.h"
#include "IGameCommandExecutor.h"
#include "Entity.h"
#include "GameCommand.h"
#include "ShootCommandArgs.h"
#include "ShootTargetCommandArgs.h"

using namespace std;
using namespace MegaManLofi;

EntityBehavior::EntityBehavior( const shared_ptr<IFrameRateProvider> frameRateProvider,
                                const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                const shared_ptr<IGameCommandExecutor> commandExecutor ) :
   _frameRateProvider( frameRateProvider ),
   _playerInfoProvider( playerInfoProvider ),
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
         RegisterFloatFromArg0( _frameRateProvider->GetFrameSeconds() );
         return true;

      case MBCGET_PLAYERPOSITIONLEFT:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetArenaPositionLeft() );
         return true;
      case MBCGET_PLAYERPOSITIONTOP:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetArenaPositionTop() );
         return true;
      case MBCGET_PLAYERVELOCITYX:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetVelocityX() );
         return true;
      case MBCGET_PLAYERVELOCITYY:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetVelocityY() );
         return true;
      case MBCGET_PLAYERDIRECTION:
         RegisterIntFromArg0( (int)_playerInfoProvider->GetPlayerEntity()->GetDirection() );
         return true;
      case MBCGET_PLAYERHEALTH:
         RegisterIntFromArg0( (int)_playerInfoProvider->GetPlayerEntity()->GetHealth() );
         return true;
      case MBCGET_PLAYERMAXHEALTH:
         RegisterIntFromArg0( (int)_playerInfoProvider->GetPlayerEntity()->GetMaxHealth() );
         return true;
      case MBCGET_PLAYERMOVEMENTTYPE:
         RegisterIntFromArg0( (int)_playerInfoProvider->GetPlayerEntity()->GetMovementType() );
         return true;
      case MBCGET_PLAYERDAMAGESECONDS:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetDamageInvulnerabilitySeconds() );
         return true;
      case MBCGET_PLAYERISINVULNERABLE:
         RegisterBoolFromArg0( _playerInfoProvider->GetPlayerEntity()->IsInvulnerable() );
         return true;

      case MBCGET_POSITIONLEFT:
         RegisterFloatFromArg0( _entity->GetArenaPositionLeft() );
         return true;
      case MBCGET_POSITIONTOP:
         RegisterFloatFromArg0( _entity->GetArenaPositionTop() );
         return true;
      case MBCGET_VELOCITYX:
         RegisterFloatFromArg0( _entity->GetVelocityX() );
         return true;
      case MBCGET_VELOCITYY:
         RegisterFloatFromArg0( _entity->GetVelocityY() );
         return true;
      case MBCGET_DIRECTION:
         RegisterIntFromArg0( (int)_entity->GetDirection() );
         return true;
      case MBCGET_HEALTH:
         RegisterIntFromArg0( (int)_entity->GetHealth() );
         return true;
      case MBCGET_MAXHEALTH:
         RegisterIntFromArg0( (int)_entity->GetMaxHealth() );
         return true;
      case MBCGET_MOVEMENTTYPE:
         RegisterIntFromArg0( (int)_entity->GetMovementType() );
         return true;
      case MBCGET_DAMAGESECONDS:
         RegisterFloatFromArg0( _entity->GetDamageInvulnerabilitySeconds() );
         return true;
      case MBCGET_ISINVULNERABLE:
         RegisterBoolFromArg0( _entity->IsInvulnerable() );
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

void EntityBehavior::RegisterFloatFromArg0( float val )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _floatRegisters[regIndex] = val;
}

void EntityBehavior::RegisterIntFromArg0( int val )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _intRegisters[regIndex] = val;
}

void EntityBehavior::RegisterBoolFromArg0( bool val )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _intRegisters[regIndex] = val ? 1 : 0;
}

void EntityBehavior::ShootTarget() const
{
   auto targetLeft = _floatRegisters[ MBC_PARSE_ARG0( _currentInstruction ) ];
   auto targetTop = _floatRegisters[ MBC_PARSE_ARG1( _currentInstruction ) ];
   _commandExecutor->ExecuteCommand( GameCommand::ShootTarget,
                                     shared_ptr<ShootTargetCommandArgs>( new ShootTargetCommandArgs( _entity, { targetLeft, targetTop } ) ) );
}
