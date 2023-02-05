#include "EnemyBehavior.h"
#include "IFrameRateProvider.h"

using namespace std;
using namespace MegaManLofi;

EnemyBehavior::EnemyBehavior( const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _frameRateProvider( frameRateProvider )
{
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
      default:
         return false;
   }
}

void EnemyBehavior::GetFrameSeconds()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _floatRegisters[regIndex] = _frameRateProvider->GetFrameSeconds();
}
