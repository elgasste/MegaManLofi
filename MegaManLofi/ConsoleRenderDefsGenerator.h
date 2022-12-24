#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameRenderDefs;
   class IFrameRateProvider;

   class ConsoleRenderDefsGenerator
   {
   public:
      static std::shared_ptr<IGameRenderDefs> GenerateConsoleRenderDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
