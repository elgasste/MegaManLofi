#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <format>

#include "ConsoleBuffer.h"
#include "ConsoleRenderDefs.h"
#include "IConsoleSprite.h"
#include "IEntityConsoleSprite.h"

namespace MegaManLofi
{
   struct ConsoleBufferInfo
   {
      ConsoleBufferInfo( HANDLE outputHandle,
                         COORD consoleSize,
                         int drawBufferSize,
                         CHAR_INFO* drawBuffer,
                         SMALL_RECT outputRect ) :
         OutputHandle( outputHandle ),
         ConsoleSize( consoleSize ),
         DrawBufferSize( drawBufferSize ),
         DrawBuffer( drawBuffer ),
         OutputRect( outputRect ) { }

      HANDLE OutputHandle;
      COORD ConsoleSize;
      int DrawBufferSize;
      CHAR_INFO* DrawBuffer;
      SMALL_RECT OutputRect;
      COORD ZeroCoordinate = { 0, 0 };
   };
}

using namespace std;
using namespace MegaManLofi;

ConsoleBuffer::ConsoleBuffer() :
   _drawBufferAllocated( false ),
   _defaultForegroundColor( (ConsoleColor)0 ),
   _defaultBackgroundColor( (ConsoleColor)0 ),
   _originalWidth( 0 ),
   _originalHeight( 0 ),
   _originalColorAttribute( 0 )
{
}

ConsoleBuffer::~ConsoleBuffer()
{
   if ( _drawBufferAllocated )
   {
      delete[] _bufferInfo->DrawBuffer;
   }
}

void ConsoleBuffer::Initialize( ConsoleColor defaultForegroundColor,
                                ConsoleColor defaultBackgroundColor,
                                short defaultWidth,
                                short defaultHeight )
{
   if ( _drawBufferAllocated )
   {
      delete[] _bufferInfo->DrawBuffer;
   }

   _defaultForegroundColor = defaultForegroundColor;
   _defaultBackgroundColor = defaultBackgroundColor;
   _originalWidth = defaultWidth;
   _originalHeight = defaultHeight;

   _bufferInfo = shared_ptr<ConsoleBufferInfo>( new ConsoleBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ),
                                                                       { _originalWidth, _originalHeight },
                                                                       _originalWidth * _originalHeight,
                                                                       new CHAR_INFO[(long long)_originalWidth * (long long)_originalHeight],
                                                                       { 0, 0, _originalWidth, _originalHeight } ) );
   _drawBufferAllocated = true;

   CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
   GetConsoleScreenBufferInfo( _bufferInfo->OutputHandle, &screenBufferInfo );
   _originalColorAttribute = screenBufferInfo.wAttributes;

   ResetDrawBuffer();
   SetCursorVisibility( false );

   Clear();
   Flip();
}

void ConsoleBuffer::ResetDrawBuffer()
{
   for ( int i = 0; i < _bufferInfo->DrawBufferSize; i++ )
   {
      _bufferInfo->DrawBuffer[i] = CHAR_INFO();
   }
}

void ConsoleBuffer::LoadRenderDefs( const shared_ptr<IGameRenderDefs> renderDefs )
{
   auto consoleDefs = static_pointer_cast<ConsoleRenderDefs>( renderDefs );

   _bufferInfo->ConsoleSize = { consoleDefs->ConsoleWidthChars, consoleDefs->ConsoleHeightChars };
   _bufferInfo->DrawBufferSize = consoleDefs->ConsoleWidthChars * consoleDefs->ConsoleHeightChars;
   _bufferInfo->DrawBuffer = new CHAR_INFO[_bufferInfo->DrawBufferSize];
   _bufferInfo->OutputRect = { 0, 0, consoleDefs->ConsoleWidthChars, consoleDefs->ConsoleHeightChars };

   ResetDrawBuffer();

   SetConsoleScreenBufferSize( _bufferInfo->OutputHandle, { _bufferInfo->ConsoleSize.X, _bufferInfo->ConsoleSize.Y } );

   SMALL_RECT windowCoords{ 0, 0, _bufferInfo->ConsoleSize.X - 1, _bufferInfo->ConsoleSize.Y - 1 };
   SetConsoleWindowInfo( _bufferInfo->OutputHandle, TRUE, &windowCoords );

   _defaultForegroundColor = consoleDefs->DefaultForegroundColor;
   _defaultBackgroundColor = consoleDefs->DefaultBackgroundColor;

   Clear();
   Flip();
}

void ConsoleBuffer::CleanUp()
{
   SetConsoleTextAttribute( _bufferInfo->OutputHandle, _originalColorAttribute );

   SMALL_RECT windowCoords{ 0, 0, _originalWidth - 1, _originalHeight - 1 };
   SetConsoleWindowInfo( _bufferInfo->OutputHandle, TRUE, &windowCoords );

   SetCursorVisibility( true );
   SetConsoleCursorPosition( _bufferInfo->OutputHandle, { 0, 0 } );

   Clear();
   Flip();
}

void ConsoleBuffer::SetDefaultForegroundColor( ConsoleColor color )
{
   _defaultForegroundColor = color;
}

void ConsoleBuffer::SetDefaultBackgroundColor( ConsoleColor color )
{
   _defaultBackgroundColor = color;
}

void ConsoleBuffer::Clear()
{
   for ( int i = 0; i < _bufferInfo->DrawBufferSize; i++ )
   {
      _bufferInfo->DrawBuffer[i].Char.AsciiChar = 0x20;
      _bufferInfo->DrawBuffer[i].Attributes = ConsoleColorsToAttribute( _defaultForegroundColor, _defaultBackgroundColor );
   }
}

void ConsoleBuffer::Draw( short left, short top, char buffer )
{
   Draw( left, top, buffer, _defaultForegroundColor );
}

void ConsoleBuffer::Draw( short left, short top, char buffer, ConsoleColor foregroundColor )
{
   Draw( left, top, buffer, foregroundColor, _defaultBackgroundColor );
}

void ConsoleBuffer::Draw( short left, short top, char buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor )
{
   if ( left < 0 || left >= _bufferInfo->ConsoleSize.X || top < 0 || top >= _bufferInfo->ConsoleSize.Y )
   {
      return;
   }

   auto bufferLocation = left + ( top * _bufferInfo->ConsoleSize.X );
   _bufferInfo->DrawBuffer[bufferLocation].Attributes = ConsoleColorsToAttribute( foregroundColor, backgroundColor );
   _bufferInfo->DrawBuffer[bufferLocation].Char.AsciiChar = buffer;
}

void ConsoleBuffer::Draw( short left, short top, const string& buffer )
{
   Draw( left, top, buffer, _defaultForegroundColor );
}

void ConsoleBuffer::Draw( short left, short top, const string& buffer, ConsoleColor foregroundColor )
{
   Draw( left, top, buffer, foregroundColor, _defaultBackgroundColor );
}

void ConsoleBuffer::Draw( short left, short top, const string& buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor )
{
   for ( int i = 0; i < (int)buffer.length(); i++ )
   {
      Draw( left + i, top, buffer[i], foregroundColor, backgroundColor );
   }
}

void ConsoleBuffer::Draw( short left, short top, const ConsoleImage& image )
{
   int i = 0, j = 0;

   for ( auto pixel : image.Pixels )
   {
      if ( pixel.HasTransparency )
      {
         if ( pixel.Value != ' ' )
         {
            Draw( left + i, top + j, pixel.Value, pixel.ForegroundColor );
         }
      }
      else
      {
         Draw( left + i, top + j, pixel.Value, pixel.ForegroundColor, pixel.BackgroundColor );
      }

      i++;

      if ( i == image.Width )
      {
         i = 0;
         j++;
      }
   }
}

void ConsoleBuffer::Draw( short left, short top, const std::shared_ptr<IConsoleSprite> sprite )
{
   Draw( left, top, sprite->GetCurrentImage() );
}

void ConsoleBuffer::Draw( short left, short top, const std::shared_ptr<IEntityConsoleSprite> sprite )
{
   Draw( left, top, sprite->GetCurrentImage() );
}

void ConsoleBuffer::Flip()
{
   WriteConsoleOutput( _bufferInfo->OutputHandle,
                       _bufferInfo->DrawBuffer,
                       _bufferInfo->ConsoleSize,
                       _bufferInfo->ZeroCoordinate,
                       &( _bufferInfo->OutputRect ) );
}

void ConsoleBuffer::SetCursorVisibility( bool isVisible )
{
   CONSOLE_CURSOR_INFO cursorInfo;
   GetConsoleCursorInfo( _bufferInfo->OutputHandle, &cursorInfo );
   cursorInfo.bVisible = isVisible;
   SetConsoleCursorInfo( _bufferInfo->OutputHandle, &cursorInfo );
}

unsigned short ConsoleBuffer::ConsoleColorsToAttribute( ConsoleColor foregroundColor, ConsoleColor backgroundColor )
{
   return (unsigned short)( (int)foregroundColor | ( (int)backgroundColor << 0x4 ) );
}
