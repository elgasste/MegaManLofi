#pragma once

#include "GameCommandArgs.h"
#include "Direction.h"

namespace MegaManLofi
{
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
