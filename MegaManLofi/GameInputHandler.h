#pragma once

#include <memory>
#include <map>

#include "IGameInputHandler.h"

namespace MegaManLofi
{
   class IGameInputReader;
   class IGameInfoProvider;
   class IGameEventAggregator;
   enum class GameState;

   class GameInputHandler : public IGameInputHandler
   {
   public:
      GameInputHandler( const std::shared_ptr<IGameInputReader> inputReader,
                        const std::shared_ptr<IGameInfoProvider> gameInfoProvider,
                        const std::shared_ptr<IGameEventAggregator> eventAggregator );

      void AddInputHandlerForGameState( GameState state,
                                        std::shared_ptr<IGameInputHandler> inputHandler );

      void HandleInput() override;

   private:
      const std::shared_ptr<IGameInputReader> _inputReader;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;

      std::map<GameState, std::shared_ptr<IGameInputHandler>> _stateInputHandlers;
   };
}
