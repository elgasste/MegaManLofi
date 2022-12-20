#pragma once

#include "Coordinate.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IConsoleAnimation
   {
   public:
      virtual void Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars ) = 0;
      virtual bool IsRunning() const = 0;
      virtual void Draw() = 0;
      virtual void Tick() = 0;
   };
}
