#pragma once

#include "IConsoleAnimation.h"

namespace MegaManLofi
{
   class StageStartedConsoleAnimation : public IConsoleAnimation
   {
   public:
      StageStartedConsoleAnimation();

      void Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick() override;

   private:
      bool _isRunning;
   };
}
