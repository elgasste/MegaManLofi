#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class ConsoleBuffer;
   class ConsoleRenderDefs;

   class GameOverStateConsoleRenderer : public IGameRenderer
   {
   public:
      GameOverStateConsoleRenderer( const std::shared_ptr<ConsoleBuffer> consoleBuffer,
                                    const std::shared_ptr<ConsoleRenderDefs> renderDefs );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<ConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
   };
}
