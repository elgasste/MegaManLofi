#pragma once

#include <memory>
#include <map>

namespace MegaManLofi
{
   class IConsoleSprite;
   class IFrameRateProvider;
   enum class Direction;

   class PlayerSpriteGenerator
   {
   public:
      static std::shared_ptr<IConsoleSprite> GenerateThwipSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GenerateThwipInTransitionSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GenerateThwipOutTransitionSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::shared_ptr<IConsoleSprite> GenerateExplosionParticleSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      static std::map<Direction, std::shared_ptr<IConsoleSprite>> GenerateStandingSpriteMap( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::map<Direction, std::shared_ptr<IConsoleSprite>> GenerateWalkingSpriteMap( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
      static std::map<Direction, std::shared_ptr<IConsoleSprite>> GenerateFallingSpriteMap( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
