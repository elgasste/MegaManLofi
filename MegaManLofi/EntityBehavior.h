#pragma once

#include <memory>

#include "MbcVirtualMachine.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IPlayerInfoProvider;
   class IGameCommandExecutor;
   class Entity;

   class EntityBehavior : public MbcVirtualMachine
   {
   public:
      EntityBehavior( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                      const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                      const std::shared_ptr<IGameCommandExecutor> commandExecutor );

      void AssignTo( const std::shared_ptr<Entity> entity );

   private:
      bool HandleCommand( mbc_command command ) override;

      void RegisterFloatFromArg0( float val );
      void RegisterIntFromArg0( int val );
      void RegisterBoolFromArg0( bool val );

   private:
      std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      std::shared_ptr<IGameCommandExecutor> _commandExecutor;

      std::shared_ptr<Entity> _entity;
   };
}
