#pragma once

#include <memory>

#include "IGame.h"
#include "IGameCommandExecutor.h"
#include "IGameInfoProvider.h"

namespace MegaManLofi
{
   class IGameEventAggregator;
   class IPlayerPhysics;
   class IArenaPhysics;

   class Game : public IGame,
                public IGameCommandExecutor,
                public IGameInfoProvider
   {
   public:
      Game( const std::shared_ptr<IGameEventAggregator> eventAggregator,
            const std::shared_ptr<IPlayerPhysics> playerPhysics,
            const std::shared_ptr<IArenaPhysics> arenaPhysics );

      void RunFrame() override;

      GameState GetGameState() const override { return _state; }

      void ExecuteCommand( GameCommand command ) override;
      void ExecuteCommand( GameCommand command, const std::shared_ptr<GameCommandArgs> args ) override;

   private:
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<IPlayerPhysics> _playerPhysics;
      const std::shared_ptr<IArenaPhysics> _arenaPhysics;

      GameState _state;
   };
}
