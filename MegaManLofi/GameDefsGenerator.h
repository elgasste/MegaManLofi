#pragma once

#include <memory>

namespace MegaManLofi
{
   class GameDefs;
   class IFrameRateProvider;
   class UniqueNumberGenerator;

   class GameDefsGenerator
   {
   public:
      static std::shared_ptr<GameDefs> GenerateGameDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                                         const std::shared_ptr<UniqueNumberGenerator> uniqueNumberGenerator );
   };
}
