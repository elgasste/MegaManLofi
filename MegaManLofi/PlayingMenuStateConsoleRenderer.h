#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;

   class PlayingMenuStateConsoleRenderer : public IGameRenderer
   {
   public:
      PlayingMenuStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                       const std::shared_ptr<ConsoleRenderConfig> renderConfig );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
   };
}
