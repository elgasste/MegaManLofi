#pragma once

#include <memory>

namespace MegaManLofi
{
   class ConsoleRenderDefs;
   class IFrameRateProvider;

   class ConsoleRenderDefsGenerator
   {
   public:
      static std::shared_ptr<ConsoleRenderDefs> GenerateConsoleRenderDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider );
   };
}
