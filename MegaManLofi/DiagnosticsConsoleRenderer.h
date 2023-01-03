#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class ConsoleBuffer;
   class IFrameRateProvider;
   class ConsoleRenderDefs;
   class IArenaInfoProvider;
   class EntityConsoleSpriteRepository;

   class DiagnosticsConsoleRenderer : public IGameRenderer
   {
   public:
      DiagnosticsConsoleRenderer( const std::shared_ptr<ConsoleBuffer> consoleBuffer,
                                  const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                  const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                                  const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                  const std::shared_ptr<EntityConsoleSpriteRepository> spriteRepository );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<ConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<EntityConsoleSpriteRepository> _spriteRepository;
   };
}
