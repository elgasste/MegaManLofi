#pragma once

#include <memory>
#include <map>

#include "Direction.h"

namespace MegaManLofi
{
   class ConsoleSpriteDefs;
   class IFrameRateProvider;
   class IConsoleSprite;
   class IEntityConsoleSprite;

   class ConsoleSpriteDefsGenerator
   {
   public:
      static std::shared_ptr<ConsoleSpriteDefs> GenerateConsoleSpriteDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

   private:
      static std::shared_ptr<IConsoleSprite> GeneratePlayerStandingSprite( Direction direction, const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GeneratePlayerWalkingSprite( Direction direction, const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GeneratePlayerAirborneSprite( Direction direction, const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<IConsoleSprite> GenerateBulletSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<IConsoleSprite> GeneratePlayerThwipSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GeneratePlayerThwipInTransitionSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GeneratePlayerThwipOutTransitionSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GeneratePlayerExplosionParticleSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<IConsoleSprite> GenerateStageGetReadySprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::shared_ptr<IConsoleSprite> GenerateMenuCaratSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::map<int, std::shared_ptr<IEntityConsoleSprite>> GenerateEntitySpriteMap( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
