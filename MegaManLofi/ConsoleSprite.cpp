#include "ConsoleSprite.h"
#include "IFrameRateProvider.h"

using namespace std;
using namespace MegaManLofi;

ConsoleSprite::ConsoleSprite( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              double imageTraversalSeconds ) :
   _frameRateProvider( frameRateProvider ),
   _currentImageIndex( 0 ),
   _imageTraversalSeconds( imageTraversalSeconds ),
   _totalSpriteSeconds( 0 ),
   _spriteElapsedSeconds( 0 )
{
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
      _spriteElapsedSeconds += _frameRateProvider->GetFrameScalar();

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
