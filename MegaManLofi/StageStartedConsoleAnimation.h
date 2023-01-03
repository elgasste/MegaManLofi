#pragma once

#include <memory>

#include "IConsoleAnimation.h"

namespace MegaManLofi
{
   class ConsoleBuffer;
   class IFrameRateProvider;
   class ConsoleRenderDefs;

   class StageStartedConsoleAnimation : public IConsoleAnimation
   {
   public:
      StageStartedConsoleAnimation( const std::shared_ptr<ConsoleBuffer> consoleBuffer,
                                    const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                    const std::shared_ptr<ConsoleRenderDefs> renderDefs );

      void Start( std::optional<Coordinate<short>> startPositionChars,
                  std::optional<Coordinate<short>> endPositionChars ) override;
      bool IsRunning() const override { return _isRunning; }
      void Draw() override;
      void Tick() override;

   private:
      const std::shared_ptr<ConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;

      bool _isRunning;
      float _elapsedSeconds;
      Coordinate<short> _positionChars;
   };
}
