#include "EnemyBehavior.h"
#include "IFrameRateProvider.h"
#include "IPlayerInfoProvider.h"
#include "ReadOnlyEntity.h"

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
         GetFrameSeconds();
         return true;
      case MBCGET_PLAYERPOSL:
         GetPlayerPositionLeft();
         return true;
      case MBCGET_PLAYERPOST:
         GetPlayerPositionTop();
         return true;
      default:
         return false;
   }
}

void EnemyBehavior::GetFrameSeconds()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _floatRegisters[regIndex] = _frameRateProvider->GetFrameSeconds();
}

void EnemyBehavior::GetPlayerPositionLeft()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _floatRegisters[regIndex] = _playerInfoProvider->GetPlayerEntity()->GetArenaPositionLeft();
}

void EnemyBehavior::GetPlayerPositionTop()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _floatRegisters[regIndex] = _playerInfoProvider->GetPlayerEntity()->GetArenaPositionTop();
}
