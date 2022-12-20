#pragma once

#include <memory>
#include <vector>

#include "IConsoleSprite.h"

namespace MegaManLofi
{
   class IFrameRateProvider;

   class ConsoleSprite : public IConsoleSprite
   {
   public:
      ConsoleSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     double imageTraversalSeconds );

      void AddImage( ConsoleImage image ) override;
      void Tick() override;
      void Reset() override;

      short GetWidth() const override;
      short GetHeight() const override;
      double GetTotalTraversalSeconds() const override { return _totalSpriteSeconds; }
      const ConsoleImage& GetCurrentImage() const override;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      std::vector<ConsoleImage> _images;
      int _currentImageIndex;
      double _imageTraversalSeconds;
      double _totalSpriteSeconds;
      double _spriteElapsedSeconds;
   };
}
