#pragma once

#include <memory>
#include <map>

#include "IGameInputHandler.h"
#include "GameState.h"

namespace MegaManLofi
{
   class IGameInputReader;
   class IGameInfoProvider;
   class GameEventAggregator;

   class GameInputHandler : public IGameInputHandler
   {
   public:
      GameInputHandler( const std::shared_ptr<IGameInputReader> inputReader,
                        const std::shared_ptr<IGameInfoProvider> gameInfoProvider,
                        const std::shared_ptr<GameEventAggregator> eventAggregator );

      void AddInputHandlerForGameState( GameState state,
                                        std::shared_ptr<IGameInputHandler> inputHandler );

      void HandleInput() override;

   private:
      const std::shared_ptr<IGameInputReader> _inputReader;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;

      std::map<GameState, std::shared_ptr<IGameInputHandler>> _stateInputHandlers;
   };
}
