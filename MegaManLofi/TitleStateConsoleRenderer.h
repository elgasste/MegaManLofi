#pragma once

#include <memory>
#include <vector>

#include "IGameRenderer.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class ConsoleBuffer;
   class IRandom;
   class IFrameRateProvider;
   class GameEventAggregator;
   class ConsoleRenderDefs;
   class KeyboardInputDefs;
   class IConsoleAnimationProvider;
   class IConsoleAnimation;

   class TitleStateConsoleRenderer : public IGameRenderer
   {
   public:
      TitleStateConsoleRenderer( const std::shared_ptr<ConsoleBuffer> consoleBuffer,
                                 const std::shared_ptr<IRandom> random,
                                 const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                 const std::shared_ptr<GameEventAggregator> eventAggregator,
                                 const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                                 const std::shared_ptr<KeyboardInputDefs> inputDefs,
                                 const std::shared_ptr<IConsoleAnimationProvider> animationProvider );

      void HandleGameStartedEvent();
      void Render() override;
      bool HasFocus() const override;

   private:
      void DrawStars();
      void DrawKeyBindings() const;

   private:
      const std::shared_ptr<ConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IRandom> _random;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
      const std::shared_ptr<KeyboardInputDefs> _inputDefs;
      const std::shared_ptr<IConsoleAnimationProvider> _animationProvider;

      const std::shared_ptr<IConsoleAnimation> _thwipOutAnimation;

      std::vector<Coordinate<float>> _starCoordinates;
      std::vector<float> _starVelocities;
   };
}
