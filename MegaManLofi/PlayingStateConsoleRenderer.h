#pragma once

#include <memory>

#include "IGameRenderer.h"
#include "Quad.h"
#include "Rectangle.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderDefs;
   class IGameInfoProvider;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;
   class IGameEventAggregator;
   class IFrameRateProvider;
   class IConsoleAnimationProvider;
   class IConsoleSprite;
   class IReadOnlyArena;

   class PlayingStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                   const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                                   const std::shared_ptr<IGameInfoProvider> gameInfoProvider,
                                   const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                   const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                   const std::shared_ptr<IGameEventAggregator> eventAggregator,
                                   const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                   const std::shared_ptr<IConsoleAnimationProvider> animationProvider );

      void Render() override;
      bool HasFocus() const override;

   private:
      void HandleStageStartedEvent();
      void HandlePitfallEvent();
      void HandleTileDeathEvent();

      void UpdateCaches();

      void DrawStageStartAnimation();
      void DrawPlayerThwipInAnimation();
      void DrawPitfallAnimation();
      void DrawPlayerExplosionAnimation();
      void DrawArenaSprites();
      void DrawPlayer();
      void DrawNonPlayerEntities();
      void DrawStatusBar();
      void DrawPauseOverlay();

      const std::shared_ptr<IConsoleSprite> GetPlayerSprite() const;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IConsoleAnimationProvider> _animationProvider;

      std::shared_ptr<IReadOnlyArena> _arena;

      Quad<long long> _viewportQuadUnits;
      Rectangle<short> _viewportRectChars;
      Coordinate<short> _viewportOffsetChars;
      Coordinate<short> _playerViewportChars;

      bool _isAnimatingPitfall;
      double _pitfallAnimationElapsedSeconds;
   };
}
