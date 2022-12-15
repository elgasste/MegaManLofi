#pragma once

#include <memory>

#include "IConsoleBuffer.h"

namespace MegaManLofi
{
   struct ConsoleBufferInfo;

   class ConsoleBuffer : public IConsoleBuffer
   {
   public:
      ConsoleBuffer();
      ~ConsoleBuffer();

      void LoadRenderConfig( const std::shared_ptr<IGameRenderConfig> renderConfig ) override;
      void CleanUp() override;

      void SetDefaultForegroundColor( ConsoleColor color ) override;
      void SetDefaultBackgroundColor( ConsoleColor color ) override;
      void Clear() override;

      void Draw( short left, short top, char buffer ) override;
      void Draw( short left, short top, char buffer, ConsoleColor foregroundColor ) override;
      void Draw( short left, short top, char buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor ) override;
      void Draw( short left, short top, const std::string& buffer ) override;
      void Draw( short left, short top, const std::string& buffer, ConsoleColor foregroundColor ) override;
      void Draw( short left, short top, const std::string& buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor ) override;
      void Draw( short left, short top, const ConsoleImage& sprite ) override;

      void Flip() override;

   private:
      void ResetDrawBuffer();
      void SetCursorVisibility( bool isVisible );
      unsigned short ConsoleColorsToAttribute( ConsoleColor foregroundColor, ConsoleColor backgroundColor );

   private:
      std::shared_ptr<ConsoleBufferInfo> _bufferInfo;

      ConsoleColor _defaultForegroundColor;
      ConsoleColor _defaultBackgroundColor;

      unsigned short _originalColorAttribute;
      short _originalWidth;
      short _originalHeight;
   };
}
