#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class IFrameRateProvider;
   class ConsoleRenderDefs;
   class IArenaInfoProvider;
   class IEntityConsoleSpriteRepository;

   class DiagnosticsConsoleRenderer : public IGameRenderer
   {
   public:
      DiagnosticsConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                  const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                  const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                                  const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                  const std::shared_ptr<IEntityConsoleSpriteRepository> spriteRepository );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<IEntityConsoleSpriteRepository> _spriteRepository;
   };
}
