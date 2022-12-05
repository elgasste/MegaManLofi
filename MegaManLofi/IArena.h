#pragma once

#include "IArenaInfoProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IArena : public IArenaInfoProvider
   {
   public:
      virtual void SetPlayerPositionX( long long positionX ) = 0;
      virtual void SetPlayerPositionY( long long positionY ) = 0;
   };
}
