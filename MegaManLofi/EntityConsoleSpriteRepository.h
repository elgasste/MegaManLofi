#pragma once

#include <map>

#include "IEntityConsoleSpriteRepository.h"

namespace MegaManLofi
{
   class IGameEventAggregator;
   class ReadOnlyArena;
   class IEntityConsoleSpriteCopier;
   class ConsoleSpriteDefs;

   class EntityConsoleSpriteRepository : public IEntityConsoleSpriteRepository
   {
   public:
      EntityConsoleSpriteRepository( const std::shared_ptr<IGameEventAggregator> eventAggregator,
                                     const std::shared_ptr<ReadOnlyArena> arena,
                                     const std::shared_ptr<IEntityConsoleSpriteCopier> spriteCopier,
                                     const std::shared_ptr<ConsoleSpriteDefs> spriteDefs );

      const std::shared_ptr<IEntityConsoleSprite> GetSprite( int uniqueId ) const override;
      int GetSpriteCount() const override { return (int)_spriteMap.size(); }

   private:
      void HandleEntitySpawned();
      void HandleEntityDeSpawned();
      void HandleEntitiesCleared();

   private:
      const std::shared_ptr<ReadOnlyArena> _arena;
      const std::shared_ptr<IEntityConsoleSpriteCopier> _spriteCopier;
      const std::shared_ptr<ConsoleSpriteDefs> _spriteDefs;

      std::map<int, std::shared_ptr<IEntityConsoleSprite>> _spriteMap;
   };
}
