#pragma once

#include <memory>

#include "MbcVirtualMachine.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IPlayerInfoProvider;
   class Entity;

   class EnemyBehavior : public MbcVirtualMachine
   {
   public:
      EnemyBehavior( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider );

      void AssignTo( const std::shared_ptr<Entity> enemy );

   private:
      bool HandleCommand( mbc_command command ) override;

      void GetFrameSeconds();
      void GetPlayerPositionLeft();
      void GetPlayerPositionTop();
      void GetPlayerVelocityX();
      void GetPlayerVelocityY();

      void SetEnemyVelocityX();
      void SetEnemyVelocityY();

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;

      std::shared_ptr<Entity> _enemy;
   };
}
