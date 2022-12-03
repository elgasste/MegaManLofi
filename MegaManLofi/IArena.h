#pragma once

#include "IArenaInfoProvider.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IArena : public IArenaInfoProvider
   {
   public:
      virtual double GetWidth() const = 0;
      virtual double GetHeight() const = 0;

      virtual double GetPlayerPositionX() const = 0;
      virtual double GetPlayerPositionY() const = 0;

      virtual void MovePlayer() = 0;
   };
}
