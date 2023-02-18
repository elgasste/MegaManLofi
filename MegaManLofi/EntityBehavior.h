#pragma once

#include <memory>

#include "MbcVirtualMachine.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;
   class IGameCommandExecutor;
   class Entity;

   class EntityBehavior : public MbcVirtualMachine
   {
   public:
      EntityBehavior( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                      const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                      const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                      const std::shared_ptr<IGameCommandExecutor> commandExecutor );

      void AssignTo( const std::shared_ptr<Entity> entity );

   private:
      bool HandleCommand( mbc_command command ) override;

      void RegisterFloatFromArg( int argNum, float val );
      void RegisterIntFromArg( int argNum, int val );
      void RegisterBoolFromArg( int argNum, bool val );

      void ShootTarget() const;

   private:
      std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      std::shared_ptr<IGameCommandExecutor> _commandExecutor;

      std::shared_ptr<Entity> _entity;
   };
}
