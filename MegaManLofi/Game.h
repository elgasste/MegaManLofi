#pragma once

#define TARGET_VECTOR_LENGTH     5000.0f

#include <memory>

#include "IGame.h"
#include "IGameCommandExecutor.h"
#include "IGameInfoProvider.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class GameEventAggregator;
   class Player;
   class Stage;
   class EntityPhysics;
   class ArenaPhysics;
   class EntityFactory;
   class EntityDefs;
   class ShootCommandArgs;
   class ShootTargetCommandArgs;

   class Game : public IGame,
                public IGameCommandExecutor,
                public IGameInfoProvider,
                public IPlayerInfoProvider,
                public IArenaInfoProvider
   {
   public:
      Game( const std::shared_ptr<GameEventAggregator> eventAggregator,
            const std::shared_ptr<Player> player,
            const std::shared_ptr<Stage> stage,
            const std::shared_ptr<EntityPhysics> entityPhysics,
            const std::shared_ptr<ArenaPhysics> arenaPhysics,
            const std::shared_ptr<EntityFactory> entityFactory,
            const std::shared_ptr<EntityDefs> entityDefs );

      void Tick() override;

      GameState GetGameState() const override { return _state; }
      bool IsPaused() const override { return _isPaused; }

      const std::shared_ptr<ReadOnlyPlayer> GetPlayer() const override;
      const std::shared_ptr<ReadOnlyEntity> GetPlayerEntity() const override;
      const std::shared_ptr<ReadOnlyArena> GetActiveArena() const override;

      void ExecuteCommand( GameCommand command ) override;
      void ExecuteCommand( GameCommand command, const std::shared_ptr<GameCommandArgs> args ) override;

   private:
      void StartGame();
      void StartStage();
      void Shoot( const std::shared_ptr<ShootCommandArgs>& args ) const;
      void Shoot( const std::shared_ptr<ShootTargetCommandArgs>& args ) const;
      void Shoot( const std::shared_ptr<ReadOnlyEntity> sourceEntity,
                  const Coordinate<float>& targetPosition ) const;
      void TogglePause();
      void OpenPlayingMenu();
      void ClosePlayingMenu();
      void HandleEnvironmentDeath();
      void HandleCollisionDeath();
      void HandleActiveArenaChanged();

   private:
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<Player> _player;
      const std::shared_ptr<Stage> _stage;
      const std::shared_ptr<EntityPhysics> _entityPhysics;
      const std::shared_ptr<ArenaPhysics> _arenaPhysics;
      const std::shared_ptr<EntityFactory> _entityFactory;
      const std::shared_ptr<EntityDefs> _entityDefs;

      GameState _state;
      GameState _nextState;

      bool _isPaused;
      bool _restartStageNextFrame;
   };
}
