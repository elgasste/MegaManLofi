#include "EnemyBehavior.h"

using namespace std;
using namespace MegaManLofi;

bool EnemyBehavior::HandleCommand( mbc_command command )
{
   if ( MbcVirtualMachine::HandleCommand( command ) )
   {
      return true;
   }

   // TODO: handle behavior-specific commands in here

   return false;
}
