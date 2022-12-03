#pragma once

#include "GameCommandArgs.h"

namespace MegaManLofi
{
   enum class Direction;

   class PushPlayerCommandArgs : public GameCommandArgs
   {
   public:
      PushPlayerCommandArgs( Direction direction ) :
         Direction ( direction )
      {
      }

      Direction Direction;
   };
}
