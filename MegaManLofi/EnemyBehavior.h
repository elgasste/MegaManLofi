#pragma once

#include "MbcVirtualMachine.h"

namespace MegaManLofi
{
   class EnemyBehavior : public MbcVirtualMachine
   {
   public:
      bool HandleCommand( mbc_command command ) override;
   };
}
