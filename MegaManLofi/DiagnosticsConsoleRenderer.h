#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class IGameClock;
   class ConsoleRenderDefs;

   class DiagnosticsConsoleRenderer : public IGameRenderer
   {
   public:
      DiagnosticsConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                  const std::shared_ptr<IGameClock> clock,
                                  const std::shared_ptr<ConsoleRenderDefs> renderDefs );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IGameClock> _clock;
      const std::shared_ptr<ConsoleRenderDefs> _renderDefs;
   };
}
