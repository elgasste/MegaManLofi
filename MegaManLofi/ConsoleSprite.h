#pragma once

#include <vector>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class ConsoleSprite
   {
   public:
      ConsoleSprite( double imageTraversalSeconds );

      void AddImage( ConsoleImage image );
      void Tick( int framesPerSecond );

      short GetWidth() const;
      short GetHeight() const;
      double GetTotalTraversalSeconds() const { return _totalSpriteSeconds; }
      const ConsoleImage& GetCurrentImage() const;

   private:
      std::vector<ConsoleImage> _images;
      int _currentImageIndex;
      double _imageTraversalSeconds;
      double _totalSpriteSeconds;
      double _spriteElapsedSeconds;
   };
}
