#pragma once

#include <memory>

#include "IGameInputHandler.h"

namespace MegaManLofi
{
   class IGameInputReader;
   class IGameCommandExecutor;
   class IMenuProvider;

   class PlayingMenuStateInputHandler : public IGameInputHandler
   {
   public:
      PlayingMenuStateInputHandler( const std::shared_ptr<IGameInputReader> inputReader,
                                    const std::shared_ptr<IGameCommandExecutor> commandExecutor,
                                    const std::shared_ptr<IMenuProvider> menuProvider );

      void HandleInput() override;

   private:
      const std::shared_ptr<IGameInputReader> _inputReader;
      const std::shared_ptr<IGameCommandExecutor> _commandExecutor;
      const std::shared_ptr<IMenuProvider> _menuProvider;
   };
}
