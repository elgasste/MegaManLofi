#pragma once

#include <memory>
#include <string>

#include "IScreenBuffer.h"
#include "ConsoleImage.h"

namespace MegaManLofi
{
   class ConsoleSprite;
   class IEntityConsoleSprite;
   struct ConsoleBufferInfo;

   class ConsoleBuffer : public IScreenBuffer
   {
   public:
      ConsoleBuffer();
      ~ConsoleBuffer();

      virtual void Initialize( ConsoleColor defaultForegroundColor,
                               ConsoleColor defaultBackgroundColor,
                               short defaultWidth,
                               short defaultHeight );
      virtual void LoadRenderDefs( const std::shared_ptr<IGameRenderDefs> renderDefs );
      virtual void CleanUp() override;

      virtual void SetDefaultForegroundColor( ConsoleColor color );
      virtual void SetDefaultBackgroundColor( ConsoleColor color );
      virtual void Clear() override;

      virtual void Draw( short left, short top, char buffer );
      virtual void Draw( short left, short top, char buffer, ConsoleColor foregroundColor );
      virtual void Draw( short left, short top, char buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor );
      virtual void Draw( short left, short top, const std::string& buffer );
      virtual void Draw( short left, short top, const std::string& buffer, ConsoleColor foregroundColor );
      virtual void Draw( short left, short top, const std::string& buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor );
      virtual void Draw( short left, short top, const ConsoleImage& image );
      virtual void Draw( short left, short top, const std::shared_ptr<ConsoleSprite> sprite );
      virtual void Draw( short left, short top, const std::shared_ptr<IEntityConsoleSprite> sprite );

      virtual void Flip();

   private:
      void ResetDrawBuffer();
      void SetCursorVisibility( bool isVisible );
      unsigned short ConsoleColorsToAttribute( ConsoleColor foregroundColor, ConsoleColor backgroundColor );

   private:
      std::shared_ptr<ConsoleBufferInfo> _bufferInfo;
      bool _drawBufferAllocated;

      ConsoleColor _defaultForegroundColor;
      ConsoleColor _defaultBackgroundColor;

      unsigned short _originalColorAttribute;
      short _originalWidth;
      short _originalHeight;
   };
}
