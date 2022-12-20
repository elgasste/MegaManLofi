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

      void Start( Coordinate<short> startCoordinate, Coordinate<short> endCoordinate ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick( int framesPerSecond ) override;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;

      bool _isRunning;
      Coordinate<short> _startCoordinate;
      Coordinate<short> _endCoordinate;
      bool _preThwipping;
      double _elapsedSeconds;
   };
}
