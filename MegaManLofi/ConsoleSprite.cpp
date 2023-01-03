#include "ConsoleSprite.h"
#include "IFrameRateProvider.h"

using namespace std;
using namespace MegaManLofi;

ConsoleSprite::ConsoleSprite() :
   _currentImageIndex( 0 ),
   _imageTraversalSeconds( 0 ),
   _totalSpriteSeconds( 0 ),
   _spriteElapsedSeconds( 0 )
{
}

ConsoleSprite::ConsoleSprite( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              float imageTraversalSeconds ) :
   _frameRateProvider( frameRateProvider ),
   _currentImageIndex( 0 ),
   _imageTraversalSeconds( imageTraversalSeconds ),
   _totalSpriteSeconds( 0 ),
   _spriteElapsedSeconds( 0 )
{
}

ConsoleSprite::ConsoleSprite( ConsoleSprite& cs ) :
   _frameRateProvider( cs._frameRateProvider ),
   _currentImageIndex( cs._currentImageIndex ),
   _imageTraversalSeconds( cs._imageTraversalSeconds ),
   _totalSpriteSeconds( cs._totalSpriteSeconds ),
   _spriteElapsedSeconds( cs._spriteElapsedSeconds )
{
   for ( auto image : cs._images )
   {
      _images.push_back( image );
   }
}

void ConsoleSprite::AddImage( ConsoleImage image )
{
   _images.push_back( image );
   _totalSpriteSeconds = _images.size() * _imageTraversalSeconds;
}

void ConsoleSprite::Tick()
{
   if ( _images.size() == 1 )
   {
      return;
   }
   else if ( _totalSpriteSeconds > 0 )
   {
      _spriteElapsedSeconds += _frameRateProvider->GetFrameSeconds();

      while ( _spriteElapsedSeconds > _totalSpriteSeconds )
      {
         _spriteElapsedSeconds -= _totalSpriteSeconds;
      }

      _currentImageIndex = (int)( _spriteElapsedSeconds / _imageTraversalSeconds );
   }
   else
   {
      _currentImageIndex++;
   }

   if ( _currentImageIndex >= _images.size() )
   {
      _currentImageIndex = 0;
   }
}

void ConsoleSprite::Reset()
{
   _currentImageIndex = 0;
   _spriteElapsedSeconds = 0;
}

short ConsoleSprite::GetWidth() const
{
   return _images[_currentImageIndex].Width;
}

short ConsoleSprite::GetHeight() const
{
   return _images[_currentImageIndex].Height;
}

const ConsoleImage& ConsoleSprite::GetCurrentImage() const
{
   return _images[_currentImageIndex];
}
