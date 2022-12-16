#pragma once

#include <memory>
#include <vector>

#include "IGameRenderer.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class IRandom;
   class IFrameRateProvider;
   class IGameEventAggregator;
   class ConsoleRenderConfig;
   class KeyboardInputConfig;

   class TitleStateConsoleRenderer : public IGameRenderer
   {
   public:
      TitleStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                 const std::shared_ptr<IRandom> random,
                                 const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                 const std::shared_ptr<IGameEventAggregator> eventAggregator,
                                 const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                 const std::shared_ptr<KeyboardInputConfig> inputConfig );

      void HandleGameStartedEvent();
      void Render() override;
      bool HasFocus() const override;

   private:
      void DrawStars();
      void DrawKeyBindings() const;
      void DrawPlayerThwipOutTransitionAnimation();
      void DrawPlayerThwipOutAnimation();
      void DrawPostThwipDelayAnimation();

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IRandom> _random;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<KeyboardInputConfig> _inputConfig;

      std::vector<Coordinate<long long>> _starCoordinates;
      std::vector<long long> _starVelocities;

      bool _isAnimatingPlayerThwipOutTransition;
      bool _isAnimatingPlayerThwipOut;
      bool _isAnimatingPostThwipDelay;
      long long _playerThwipBottomUnits;
      double _preThwipElapsedSeconds;
      double _postThwipElapsedSeconds;
   };
}
