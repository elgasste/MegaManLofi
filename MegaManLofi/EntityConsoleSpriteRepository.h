#pragma once

#include <memory>
#include <map>

namespace MegaManLofi
{
   class GameEventAggregator;
   class IArenaInfoProvider;
   class EntityConsoleSprite;
   class EntityConsoleSpriteCopier;
   class ConsoleSpriteDefs;

   class EntityConsoleSpriteRepository
   {
   public:
      EntityConsoleSpriteRepository( const std::shared_ptr<GameEventAggregator> eventAggregator,
                                     const std::shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                     const std::shared_ptr<EntityConsoleSpriteCopier> spriteCopier,
                                     const std::shared_ptr<ConsoleSpriteDefs> spriteDefs );

      virtual const std::shared_ptr<EntityConsoleSprite> GetSprite( int uniqueId ) const;
      virtual int GetSpriteCount() const { return (int)_spriteMap.size(); }

   private:
      void HandleEntitySpawned();
      void HandleEntityDeSpawned();
      void HandleEntitiesCleared();

   private:
      const std::shared_ptr<IArenaInfoProvider> _arenaInfoProvider;
      const std::shared_ptr<EntityConsoleSpriteCopier> _spriteCopier;
      const std::shared_ptr<ConsoleSpriteDefs> _spriteDefs;

      std::map<int, std::shared_ptr<EntityConsoleSprite>> _spriteMap;
   };
}
