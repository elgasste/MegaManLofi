#pragma once

#include "GameCommandArgs.h"

namespace MegaManLofi
{
   enum class Direction;

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
