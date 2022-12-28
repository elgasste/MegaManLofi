#pragma once

#include <map>

#include "IEntityConsoleSpriteRepository.h"

namespace MegaManLofi
{
   class IGameEventAggregator;
   class IReadOnlyArena;
   class ConsoleSpriteDefs;

   class EntityConsoleSpriteRepository : public IEntityConsoleSpriteRepository
   {
   public:
      EntityConsoleSpriteRepository( const std::shared_ptr<IGameEventAggregator> eventAggregator,
                                     const std::shared_ptr<IReadOnlyArena> arena,
                                     const std::shared_ptr<ConsoleSpriteDefs> spriteDefs );

      const std::shared_ptr<IEntityConsoleSprite> GetSprite( int uniqueId ) const override;

   private:
      void HandleEntitySpawned();
      void HandleEntityDeSpawned();
      void HandleEntitiesCleared();

   private:
      const std::shared_ptr<IReadOnlyArena> _arena;
      const std::shared_ptr<ConsoleSpriteDefs> _spriteDefs;

      std::map<int, std::shared_ptr<IEntityConsoleSprite>> _spriteMap;
   };
}
