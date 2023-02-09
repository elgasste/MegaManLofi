#pragma once

#include <memory>

#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   class EntityDefs;
   class UniqueNumberGenerator;
   class IFrameRateProvider;
   class IPlayerInfoProvider;
   class IGameCommandExecutor;
   class Entity;

   class EntityFactory
   {
   public:
      EntityFactory() { }
      EntityFactory( const std::shared_ptr<EntityDefs> entityDefs,
                     const std::shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator,
                     const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      void Initialize( const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                       const std::shared_ptr<IGameCommandExecutor> commandExecutor );

      virtual const std::shared_ptr<Entity> CreateEntity( int entityMetaId, Direction direction ) const;

   private:
      void SetItemInfo( const std::shared_ptr<Entity> item, int metaId ) const;
      void SetProjectileInfo( const std::shared_ptr<Entity> projectile, Direction direction ) const;
      void SetEnemyInfo( const std::shared_ptr<Entity> enemy, int metaId ) const;

   private:
      const std::shared_ptr<EntityDefs> _entityDefs;
      const std::shared_ptr<UniqueNumberGenerator> _uniqueNumberGenerator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      std::shared_ptr<IGameCommandExecutor> _commandExecutor;
   };
}
