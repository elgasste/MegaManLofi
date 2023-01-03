#pragma once

#include <memory>

#include "IGameRenderer.h"
#include "Quad.h"
#include "Rectangle.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class ConsoleBuffer;
   class ConsoleRenderDefs;
   class IGameInfoProvider;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;
   class GameEventAggregator;
   class IFrameRateProvider;
   class IConsoleAnimationProvider;
   class EntityConsoleSpriteRepository;
   class ConsoleSprite;
   class ReadOnlyArena;
   class ReadOnlyEntity;

   class PlayingStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingStateConsoleRenderer( const std::shared_ptr<ConsoleBuffer> consoleBuffer,
                                   const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                                   const std::shared_ptr<IGameInfoProvider> gameInfoProvider,
                                   const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                   const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                   const std::shared_ptr<GameEventAggregator> eventAggregator,
                                   const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                   const std::shared_ptr<IConsoleAnimationProvider> animationProvider,
                                   const std::shared_ptr<EntityConsoleSpriteRepository> spriteRepository );

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
      void DrawEntity( const std::shared_ptr<ReadOnlyEntity> entity );
      void DrawStatusBar();
      void DrawPauseOverlay();

   private:
      const std::shared_ptr<ConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IConsoleAnimationProvider> _animationProvider;
      const std::shared_ptr<EntityConsoleSpriteRepository> _spriteRepository;

      std::shared_ptr<ReadOnlyArena> _arena;

      Quad<float> _viewportQuadUnits;
      Rectangle<short> _viewportRectChars;
      Coordinate<short> _viewportOffsetChars;
      Coordinate<short> _playerViewportChars;

      bool _isAnimatingPitfall;
      float _pitfallAnimationElapsedSeconds;
   };
}
