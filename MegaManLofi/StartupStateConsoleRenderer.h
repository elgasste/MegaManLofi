#pragma once

#include <memory>
#include <vector>

#include "IGameRenderer.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class IConsoleBuffer;
   class IRandom;
   class IFrameRateProvider;
   class ConsoleRenderConfig;
   class KeyboardInputConfig;

   class StartupStateConsoleRenderer : public IGameRenderer
   {
   public:
      StartupStateConsoleRenderer( const std::shared_ptr<IConsoleBuffer> consoleBuffer,
                                   const std::shared_ptr<IRandom> random,
                                   const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                                   const std::shared_ptr<ConsoleRenderConfig> renderConfig,
                                   const std::shared_ptr<KeyboardInputConfig> inputConfig );

      void Render() override;
      bool HasFocus() const override { return false; }

   private:
      void DrawStars();
      void DrawKeyBindings() const;

   private:
      const std::shared_ptr<IConsoleBuffer> _consoleBuffer;
      const std::shared_ptr<IRandom> _random;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<ConsoleRenderConfig> _renderConfig;
      const std::shared_ptr<KeyboardInputConfig> _inputConfig;

      std::vector<Coordinate> _starCoordinates;
      std::vector<long long> _starVelocities;
   };
}
