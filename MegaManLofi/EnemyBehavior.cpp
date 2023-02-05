#include "EnemyBehavior.h"
#include "IFrameRateProvider.h"
#include "IPlayerInfoProvider.h"
#include "Entity.h"

using namespace std;
using namespace MegaManLofi;

EnemyBehavior::EnemyBehavior( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              const shared_ptr<IPlayerInfoProvider> playerInfoProvider ) :
   _frameRateProvider( frameRateProvider ),
   _playerInfoProvider( playerInfoProvider ),
   _enemy( nullptr )
{
}

void EnemyBehavior::AssignTo( const std::shared_ptr<Entity> enemy )
{
   _enemy = enemy;
}

bool EnemyBehavior::HandleCommand( mbc_command command )
{
   if ( MbcVirtualMachine::HandleCommand( command ) )
   {
      return true;
   }

   switch ( command )
   {
      case MBCGET_FRAMESECS:
         RegisterFloatFromArg0( _frameRateProvider->GetFrameSeconds() );
         return true;
      case MBCGET_PLAYERPOSL:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetArenaPositionLeft() );
         return true;
      case MBCGET_PLAYERPOST:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetArenaPositionTop() );
         return true;
      case MBCGET_PLAYERVELX:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetVelocityX() );
         return true;
      case MBCGET_PLAYERVELY:
         RegisterFloatFromArg0( _playerInfoProvider->GetPlayerEntity()->GetVelocityY() );
         return true;
      case MBCGET_PLAYERDIR:
         RegisterIntFromArg0( (int)_playerInfoProvider->GetPlayerEntity()->GetDirection() );
         return true;

      case MBCSET_ENTVELX:
         _enemy->SetVelocityX( _floatRegisters[MBC_PARSE_ARG0( _currentInstruction )] );
         return true;
      case MBCSET_ENTVELY:
         _enemy->SetVelocityY( _floatRegisters[MBC_PARSE_ARG0( _currentInstruction )] );
         return true;

      default:
         return false;
   }
}

void EnemyBehavior::RegisterFloatFromArg0( float val )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _floatRegisters[regIndex] = val;
}

void EnemyBehavior::RegisterIntFromArg0( int val )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _intRegisters[regIndex] = val;
}
