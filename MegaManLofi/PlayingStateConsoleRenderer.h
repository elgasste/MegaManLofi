#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;
   class IGameEventAggregator;

   class PlayingStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                   const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                   const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                   const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                   const std::shared_ptr<IGameEventAggregator> eventAggregator );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      void HandleGameStartedEvent();
      void DrawArenaFence();
      void DrawArenaSprites();
      void DrawPlayer();

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;

      double _arenaCoordConverterX;
      double _arenaCoordConverterY;
   };
}
