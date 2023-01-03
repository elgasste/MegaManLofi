#pragma once

#include <memory>

#include "IGame.h"
#include "IGameCommandExecutor.h"
#include "IGameInfoProvider.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"

namespace MegaManLofi
{
   class IGameEventAggregator;
   class Player;
   class Arena;
   class IPlayerPhysics;
   class IArenaPhysics;
   class IEntityFactory;

   class Game : public IGame,
                public IGameCommandExecutor,
                public IGameInfoProvider,
                public IPlayerInfoProvider,
                public IArenaInfoProvider
   {
   public:
      Game( const std::shared_ptr<IGameEventAggregator> eventAggregator,
            const std::shared_ptr<Player> player,
            const std::shared_ptr<Arena> arena,
            const std::shared_ptr<IPlayerPhysics> playerPhysics,
            const std::shared_ptr<IArenaPhysics> arenaPhysics,
            const std::shared_ptr<IEntityFactory> entityFactory );

      void Tick() override;

      GameState GetGameState() const override { return _state; }
      bool IsPaused() const override { return _isPaused; }

      const std::shared_ptr<ReadOnlyPlayer> GetPlayer() const override;
      const std::shared_ptr<ReadOnlyEntity> GetPlayerEntity() const override;
      const std::shared_ptr<ReadOnlyArena> GetArena() const override;

      void ExecuteCommand( GameCommand command ) override;
      void ExecuteCommand( GameCommand command, const std::shared_ptr<GameCommandArgs> args ) override;

   private:
      void StartStage();
      void Shoot();
      void TogglePause();
      void OpenPlayingMenu();
      void ClosePlayingMenu();
      void KillPlayer();

   private:
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<Player> _player;
      const std::shared_ptr<Arena> _arena;
      const std::shared_ptr<IPlayerPhysics> _playerPhysics;
      const std::shared_ptr<IArenaPhysics> _arenaPhysics;
      const std::shared_ptr<IEntityFactory> _entityFactory;

      GameState _state;
      GameState _nextState;

      bool _isPaused;
      bool _restartStageNextFrame;
   };
}
