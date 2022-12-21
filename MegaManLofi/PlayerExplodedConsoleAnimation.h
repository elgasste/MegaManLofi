#pragma once

#include <memory>

#include "IConsoleAnimation.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class IFrameRateProvider;
   class ConsoleRenderConfig;

   class PlayerExplodedConsoleAnimation : public IConsoleAnimation
   {
   public:
      PlayerExplodedConsoleAnimation( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                      const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                      const std::shared_ptr<ConsoleRenderConfig> renderConfig );

      void Start( std::optional<Coordinate<short>> startPositionChars,
                  std::optional<Coordinate<short>> endPositionChars ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick() override;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;

      Coordinate<short> _startPositionChars;
      bool _isRunning;
      double _elapsedSeconds;
      long long _explosionStartFrame;
   };
}
