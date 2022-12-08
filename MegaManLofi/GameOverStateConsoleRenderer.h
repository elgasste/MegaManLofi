#pragma once

#include <memory>

#include "IGameRenderer.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class ConsoleRenderConfig;
   class KeyboardInputConfig;

   class GameOverStateConsoleRenderer : public IGameRenderer
   {
   public:
      GameOverStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                    const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                    const std::shared_ptr<KeyboardInputConfig> inputConfig );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<KeyboardInputConfig> _inputConfig;
   };
}
