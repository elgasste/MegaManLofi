#pragma once

#include <vector>

#include "IConsoleSprite.h"

namespace MegaManLofi
{
   class ConsoleSprite : public IConsoleSprite
   {
   public:
      ConsoleSprite( double imageTraversalSeconds );

      void AddImage( ConsoleImage image ) override;
      void Tick( int framesPerSecond ) override;
      void Reset() override;

      short GetWidth() const override;
      short GetHeight() const override;
      double GetTotalTraversalSeconds() const override { return _totalSpriteSeconds; }
      const ConsoleImage& GetCurrentImage() const override;

   private:
      std::vector<ConsoleImage> _images;
      int _currentImageIndex;
      double _imageTraversalSeconds;
      double _totalSpriteSeconds;
      double _spriteElapsedSeconds;
   };
}
