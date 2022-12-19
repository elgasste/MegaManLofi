#pragma once

#include "Coordinate.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IConsoleAnimation
   {
   public:
      virtual void Start( Coordinate<short> startCoordinate, Coordinate<short> endCoordinate ) = 0;
      virtual bool IsRunning() const = 0;
      virtual void Draw() = 0;
      virtual void Tick( int framesPerSecond ) = 0;
   };
}
