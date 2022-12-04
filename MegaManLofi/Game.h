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
   class IPlayer;

   class Game : public IGame,
                public IGameCommandExecutor,
                public IGameInfoProvider
   {
   public:
      Game( const std::shared_ptr<IGameEventAggregator> eventAggregator,
            const std::shared_ptr<IPlayerPhysics> playerPhysics,
            const std::shared_ptr<IArenaPhysics> arenaPhysics,
            const std::shared_ptr<IPlayer> player );

      void RunFrame() override;

      GameState GetGameState() const override { return _state; }

      void ExecuteCommand( GameCommand command ) override;
      void ExecuteCommand( GameCommand command, const std::shared_ptr<GameCommandArgs> args ) override;

   private:
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<IPlayerPhysics> _playerPhysics;
      const std::shared_ptr<IArenaPhysics> _arenaPhysics;
      const std::shared_ptr<IPlayer> _player;

      GameState _state;
   };
}
