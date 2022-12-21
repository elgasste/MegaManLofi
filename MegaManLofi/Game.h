#pragma once

#include <memory>

#include "IGame.h"
#include "IGameCommandExecutor.h"
#include "IGameInfoProvider.h"

namespace MegaManLofi
{
   class IGameEventAggregator;
   class IPlayer;
   class IArena;
   class IPlayerPhysics;
   class IArenaPhysics;

   class Game : public IGame,
                public IGameCommandExecutor,
                public IGameInfoProvider
   {
   public:
      Game( const std::shared_ptr<IGameEventAggregator> eventAggregator,
            const std::shared_ptr<IPlayer> player,
            const std::shared_ptr<IArena> arena,
            const std::shared_ptr<IPlayerPhysics> playerPhysics,
            const std::shared_ptr<IArenaPhysics> arenaPhysics );

      void Tick() override;

      GameState GetGameState() const override { return _state; }
      bool IsPaused() const override { return _isPaused; }

      void ExecuteCommand( GameCommand command ) override;
      void ExecuteCommand( GameCommand command, const std::shared_ptr<GameCommandArgs> args ) override;

   private:
      void StartStage();
      void TogglePause();
      void OpenPlayingMenu();
      void ClosePlayingMenu();
      void KillPlayer();

   private:
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<IPlayer> _player;
      const std::shared_ptr<IArena> _arena;
      const std::shared_ptr<IPlayerPhysics> _playerPhysics;
      const std::shared_ptr<IArenaPhysics> _arenaPhysics;

      GameState _state;
      GameState _nextState;

      bool _isPaused;
      bool _restartStageNextFrame;
   };
}
