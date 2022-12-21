#pragma once

#include <memory>

#include "IConsoleAnimation.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;
   class IFrameRateProvider;

   class PlayerThwipInConsoleAnimation : public IConsoleAnimation
   {
   public:
      PlayerThwipInConsoleAnimation( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                     const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                     const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      void Start( std::optional<Coordinate<short>> startPositionChars,
                  std::optional<Coordinate<short>> endPositionChars ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick() override;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      bool _isRunning;
      Coordinate<short> _startPositionChars;
      Coordinate<short> _endPositionChars;
      long long _currentTopPositionUnits;
      long long _endTopPositionUnits;
      bool _postThwipping;
      double _elapsedSeconds;
   };
}
