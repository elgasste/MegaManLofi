#pragma once

#include <memory>

#include "IGameRenderer.h"
#include "Rectangle.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;
   class IGameInfoProvider;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;
   class IGameEventAggregator;
   class IFrameRateProvider;

   class PlayingStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                   const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                   const std::shared_ptr<IGameInfoProvider> gameInfoProvider,
                                   const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                   const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                   const std::shared_ptr<IGameEventAggregator> eventAggregator,
                                   const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      void Render() override;
      bool HasFocus() const override;

   private:
      void HandleStageStartedEvent();
      void HandlePitfallEvent();
      void HandleTileDeathEvent();

      void UpdateCaches();

      void DrawGameStartAnimation();
      void DrawPlayerThwipInAnimation();
      void DrawPitfallAnimation();
      void DrawPlayerExplosionAnimation();
      void DrawArenaSprites();
      void DrawPlayer();
      void DrawStatusBar();
      void DrawPauseOverlay();

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      Rectangle<short> _viewportRectChars;
      Coordinate<short> _viewportOffsetChars;
      Coordinate<short> _playerViewportChars;

      bool _isAnimatingStageStart;
      bool _isAnimatingPlayerThwipIn;
      bool _isAnimatingPitfall;
      bool _isAnimatingPlayerExplosion;

      double _stageStartAnimationElapsedSeconds;
      double _pitfallAnimationElapsedSeconds;
      double _playerExplosionAnimationElapsedSeconds;

      long long _playerThwipBottom;
      long long _playerExplosionStartFrame;
   };
}
