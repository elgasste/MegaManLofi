#pragma once

#include <memory>

#include "IConsoleAnimation.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;

   class PlayerThwipOutConsoleAnimation : public IConsoleAnimation
   {
   public:
      PlayerThwipOutConsoleAnimation( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                      const std::shared_ptr<ConsoleRenderConfig> renderConfig );

      void Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick( int framesPerSecond ) override;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;

      bool _isRunning;
      Coordinate<short> _startPositionChars;
      Coordinate<short> _endPositionChars;
      long long _currentTopPositionUnits;
      long long _endTopPositionUnits;
      bool _preThwipping;
      double _elapsedSeconds;
   };
}
