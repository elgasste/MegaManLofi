#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderDefs;

   class GameOverStateConsoleRenderer : public IGameRenderer
   {
   public:
      GameOverStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                    const std::shared_ptr<ConsoleRenderDefs> renderDefs );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
   };
}
