#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;

   class PlayingStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                   const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                   const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                   const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      void DrawArenaFence();
      void DrawArenaSprites();
      void DrawPlayer();

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;

      double _arenaCoordConverterX;
      double _arenaCoordConverterY;
   };
}
