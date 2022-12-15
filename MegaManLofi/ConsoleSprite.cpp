#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

ConsoleSprite::ConsoleSprite( double imageTraversalSeconds ) :
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

void ConsoleSprite::Tick( int framesPerSecond )
{
   _spriteElapsedSeconds += ( 1 / (double)framesPerSecond );

   while ( _spriteElapsedSeconds > _totalSpriteSeconds )
   {
      _spriteElapsedSeconds -= _totalSpriteSeconds;
   }

   _currentImageIndex = (int)( _spriteElapsedSeconds / _imageTraversalSeconds );

   // this can happen if elapsed seconds exactly matches total sprite seconds
   if ( _currentImageIndex >= _images.size() )
   {
      _currentImageIndex = 0;
   }
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
