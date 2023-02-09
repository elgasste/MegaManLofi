#pragma once

#include <memory>
#include <map>

#include "Direction.h"

namespace MegaManLofi
{
   class ConsoleSpriteDefs;
   class IFrameRateProvider;
   class ConsoleSprite;
   class EntityConsoleSprite;

   class ConsoleSpriteDefsGenerator
   {
   public:
      static std::shared_ptr<ConsoleSpriteDefs> GenerateConsoleSpriteDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

   private:
      static std::shared_ptr<ConsoleSprite> GeneratePlayerStandingSprite( Direction direction, const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GeneratePlayerWalkingSprite( Direction direction, const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GeneratePlayerAirborneSprite( Direction direction, const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<ConsoleSprite> GenerateBulletSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GenerateSmallHealthDropSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GenerateLargeHealthDropSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GenerateExtraLifeSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<ConsoleSprite> GenerateStationaryTurretSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GenerateSpinningTurretSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<ConsoleSprite> GeneratePlayerThwipSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GeneratePlayerThwipInTransitionSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GeneratePlayerThwipOutTransitionSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<ConsoleSprite> GeneratePlayerExplosionParticleSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<ConsoleSprite> GenerateStageGetReadySprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<ConsoleSprite> GenerateMenuCaratSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::map<int, std::shared_ptr<EntityConsoleSprite>> GenerateEntitySpriteMap( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
