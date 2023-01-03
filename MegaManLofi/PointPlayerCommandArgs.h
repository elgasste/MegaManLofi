#pragma once

#include "GameCommandArgs.h"
#include "Direction.h"

namespace MegaManLofi
{
   class PointPlayerCommandArgs : public GameCommandArgs
   {
   public:
      PointPlayerCommandArgs( Direction direction ) :
         Direction ( direction )
      {
      }

      Direction Direction;
   };
}
