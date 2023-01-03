#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class ConsoleBuffer;
   class ConsoleRenderDefs;
   class IMenuProvider;

   class PlayingMenuStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingMenuStateConsoleRenderer( const std::shared_ptr<ConsoleBuffer> consoleBuffer,
                                       const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                                       const std::shared_ptr<IMenuProvider> menuProvider );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<ConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
      const std::shared_ptr<IMenuProvider> _menuProvider;
   };
}
