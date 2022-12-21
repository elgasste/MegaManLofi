#pragma once

#include <memory>

#include "IGameInputHandler.h"

namespace MegaManLofi
{
   class IGameInputReader;
   class IGameCommandExecutor;

   class PlayingMenuStateInputHandler : public IGameInputHandler
   {
   public:
      PlayingMenuStateInputHandler( const std::shared_ptr<IGameInputReader> inputReader,
                                    const std::shared_ptr<IGameCommandExecutor> commandExecutor );

      void HandleInput() override;

   private:
      const std::shared_ptr<IGameInputReader> _inputReader;
      const std::shared_ptr<IGameCommandExecutor> _commandExecutor;
   };
}
