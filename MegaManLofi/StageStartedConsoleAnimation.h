#pragma once

#include <memory>

#include "IConsoleAnimation.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class IFrameRateProvider;
   class ConsoleRenderConfig;

   class StageStartedConsoleAnimation : public IConsoleAnimation
   {
   public:
      StageStartedConsoleAnimation( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                    const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                    const std::shared_ptr<ConsoleRenderConfig> renderConfig );

      void Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick() override;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;

      bool _isRunning;
      double _elapsedSeconds;
      Coordinate<short> _positionChars;
   };
}
