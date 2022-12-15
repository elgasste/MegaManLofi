#pragma once

#include <memory>
#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IFrameRateProvider;

   class ConsoleSprite
   {
   public:
      ConsoleSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     double imageTraversalSeconds );

      void AddImage( ConsoleImage image );
      void Tick();

      short GetWidth() const;
      short GetHeight() const;
      const ConsoleImage& GetCurrentImage() const;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      std::vector<ConsoleImage> _images;
      int _currentImageIndex;
      double _imageTraversalSeconds;
      double _totalSpriteSeconds;
      double _spriteElapsedSeconds;
   };
}
