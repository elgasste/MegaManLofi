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
      ConsoleSprite();
      ConsoleSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     float imageTraversalSeconds );
      ConsoleSprite( ConsoleSprite& cs );

      virtual void AddImage( ConsoleImage image );
      virtual void Tick();
      virtual void Reset();

      virtual short GetWidth() const;
      virtual short GetHeight() const;
      virtual float GetTotalTraversalSeconds() const { return _totalSpriteSeconds; }
      virtual const ConsoleImage& GetCurrentImage() const;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      std::vector<ConsoleImage> _images;
      int _currentImageIndex;
      float _imageTraversalSeconds;
      float _totalSpriteSeconds;
      float _spriteElapsedSeconds;
   };
}
