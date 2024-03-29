#pragma once

#include <memory>

#include "EntityType.h"
#include "Coordinate.h"
#include "Direction.h"

namespace MegaManLofi
{
   class EntityDefs;
   class UniqueNumberGenerator;
   class IFrameRateProvider;
   class IRandom;
   class IPlayerInfoProvider;
   class IArenaInfoProvider;
   class IGameCommandExecutor;
   class Entity;

   class EntityFactory
   {
   public:
      EntityFactory() { }
      EntityFactory( const std::shared_ptr<EntityDefs> entityDefs,
                     const std::shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator,
                     const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<IRandom> random );

      void Initialize( const std::shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                       const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                       const std::shared_ptr<IGameCommandExecutor> commandExecutor );

      virtual const std::shared_ptr<Entity> CreateEntity( int entityMetaId,
                                                          const Coordinate<float>& position,
                                                          Direction direction ) const;
      virtual const std::shared_ptr<Entity> CreateItem( int itemMetaId,
                                                        const Coordinate<float>& position,
                                                        Direction direction ) const;
      virtual const std::shared_ptr<Entity> CreateProjectile( int projectileMetaId,
                                                              const Coordinate<float>& position,
                                                              Direction direction ) const;
      virtual const std::shared_ptr<Entity> CreateTargetedProjectile( int projectileMetaId,
                                                                      const Coordinate<float>& position,
                                                                      const Coordinate<float>& target ) const;
      virtual const std::shared_ptr<Entity> CreateEnemy( int enemyMetaId,
                                                         const Coordinate<float>& position,
                                                         Direction direction ) const;

   private:
      void SetEntityBaseInfo( std::shared_ptr<Entity> entity,
                              EntityType type,
                              int entityMetaId,
                              const Coordinate<float>& position,
                              Direction direction ) const;
      void SetProjectileBaseInfo( std::shared_ptr<Entity> projectile, int projectileMetaId ) const;

   private:
      const std::shared_ptr<EntityDefs> _entityDefs;
      const std::shared_ptr<UniqueNumberGenerator> _uniqueNumberGenerator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IRandom> _random;

      std::shared_ptr<IPlayerInfoProvider> _playerInfoProvider;
      std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      std::shared_ptr<IGameCommandExecutor> _commandExecutor;
   };
}
