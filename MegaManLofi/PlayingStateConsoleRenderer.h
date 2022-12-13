#pragma once

#include <memory>

#include "IGameRenderer.h"

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
      void CalculateViewportOffsets();
      void DrawGameStartAnimation();
      void DrawPitfallAnimation();
      void DrawPlayerExplosionAnimation();
      void DrawArenaSprites();
      void DrawPlayer();
      void DrawStatusBar();
      void DrawPauseOverlay();

      short GetPlayerViewportX() const;
      short GetPlayerViewportY() const;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      long long _viewportWidth;
      long long _viewportHeight;
      long long _viewportOffsetX;
      long long _viewportOffsetY;

      bool _isAnimatingStageStart;
      bool _isAnimatingPitfall;
      bool _isAnimatingPlayerExplosion;

      double _stageStartAnimationElapsedSeconds;
      double _pitfallAnimationElapsedSeconds;
      double _playerExplosionAnimationElapsedSeconds;

      long long _playerExplosionStartFrame;
   };
}
