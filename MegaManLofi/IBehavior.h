#pragma once

#include <vector>

#include "MbcDefines.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IBehavior
   {
   public:
      virtual void SetInstructions( std::vector<mbc_instruction> instructions ) = 0;
      virtual void Tick() = 0;
   };
}
