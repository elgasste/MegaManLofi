#pragma once

#include "IArenaInfoProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IArena : public IArenaInfoProvider
   {
   public:
      virtual void SetPlayerPositionX( double positionX ) = 0;
      virtual void SetPlayerPositionY( double positionY ) = 0;
   };
}
