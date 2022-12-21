#pragma once

#include <optional>

#include "Coordinate.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IConsoleAnimation
   {
   public:
      virtual void Start( std::optional<Coordinate<short>> startPositionChars,
                          std::optional<Coordinate<short>> endPositionChars ) = 0;
      virtual bool IsRunning() const = 0;
      virtual void Draw() = 0;
      virtual void Tick() = 0;
   };
}
